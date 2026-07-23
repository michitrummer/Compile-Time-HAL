/**
 * @file GpioConfig.h
 * @brief Compile-time GPIO configuration aggregation.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL23_GPIO_CONFIG_H_
#define CSL23_GPIO_CONFIG_H_

#include <cstddef>
#include <cstdint>
#include <utility>

#include "./Pin.h"
#include "./RCC.h"

namespace csl {

/**
 * Structural C++20/23 value type used directly as a non-type template argument.
 */
/**
 * @brief Structural GPIO configuration value usable as a C++20/23 NTTP.
 *
 * The value is consumed entirely during template instantiation; no runtime
 * configuration object is required by PortConfiguration.
 */
struct PinConfig {
  pin::Id id;
  pin::Mode mode;
  pin::Pull pull = pin::Pull::noPull;
  pin::Speed speed = pin::Speed::freqLow;

  constexpr bool operator==(const PinConfig&) const = default;
};

namespace detail {

consteval bool validPin(PinConfig config) {
  return std::to_underlying(config.id) <= std::to_underlying(pin::Id::pin15);
}

consteval bool validMode(PinConfig config) {
  return config.mode == pin::Mode::inp || config.mode == pin::Mode::outPP ||
         config.mode == pin::Mode::outOD;
}

consteval bool validPull(PinConfig config) {
  return config.pull == pin::Pull::noPull || config.pull == pin::Pull::pullUp ||
         config.pull == pin::Pull::pullDown;
}

consteval bool validSpeed(PinConfig config) {
  return config.speed == pin::Speed::freqLow ||
         config.speed == pin::Speed::freqMid ||
         config.speed == pin::Speed::freqHigh ||
         config.speed == pin::Speed::freqVeryHigh;
}

consteval bool validPinConfiguration(PinConfig config) {
  return validPin(config) && validMode(config) && validPull(config) &&
         validSpeed(config);
}

consteval bool isOutput(PinConfig config) {
  return config.mode == pin::Mode::outPP || config.mode == pin::Mode::outOD;
}

consteval std::uint32_t pinBit(PinConfig config) {
  return 1u << std::to_underlying(config.id);
}

consteval std::uint32_t shift(PinConfig config) {
  return std::to_underlying(config.id) * 2u;
}

consteval std::uint32_t modeMask(PinConfig config) {
  return 3u << shift(config);
}

consteval std::uint32_t modeValue(PinConfig config) {
  return (std::to_underlying(config.mode) & 3u) << shift(config);
}

consteval std::uint32_t pullMask(PinConfig config) {
  return 3u << shift(config);
}

consteval std::uint32_t pullValue(PinConfig config) {
  return std::to_underlying(config.pull) << shift(config);
}

consteval std::uint32_t typeMask(PinConfig config) {
  return isOutput(config) ? pinBit(config) : 0u;
}

consteval std::uint32_t typeValue(PinConfig config) {
  return config.mode == pin::Mode::outOD ? pinBit(config) : 0u;
}

consteval std::uint32_t speedMask(PinConfig config) {
  return isOutput(config) ? (3u << shift(config)) : 0u;
}

consteval std::uint32_t speedValue(PinConfig config) {
  return isOutput(config) ? (std::to_underlying(config.speed) << shift(config))
                          : 0u;
}

template <PinConfig... Configs>
consteval bool pinsAreUnique() {
  if constexpr (sizeof...(Configs) < 2u) {
    return true;
  } else {
    constexpr PinConfig configs[] = {Configs...};
    for (std::size_t first = 0; first < sizeof...(Configs); ++first) {
      for (std::size_t second = first + 1u; second < sizeof...(Configs);
           ++second) {
        if (configs[first].id == configs[second].id) return false;
      }
    }
    return true;
  }
}

}  // namespace detail

/** @brief Accepts only pin configurations representable by the STM32L433 GPIO
 * registers. */
template <PinConfig Config>
concept ValidPinConfiguration = detail::validPinConfiguration(Config);

/**
 * @brief Aggregates C++23 PinConfig values into one configuration per GPIO
 * port.
 * @tparam Id GPIO port selected at compile time.
 * @tparam Configs Structural PinConfig values belonging to the port.
 *
 * Fold expressions combine masks without template recursion, while consteval
 * validation rejects unsupported values and duplicate pin assignments.
 */
template <port::Id Id, PinConfig... Configs>
  requires SupportedPort<Id> && (ValidPinConfiguration<Configs> && ...)
struct PortConfiguration {
  static_assert(
      detail::pinsAreUnique<Configs...>(),
      "A GPIO pin is configured more than once in one port configuration");

  inline static constexpr std::uint32_t modeMask =
      (0u | ... | detail::modeMask(Configs));
  inline static constexpr std::uint32_t modeValue =
      (0u | ... | detail::modeValue(Configs));
  inline static constexpr std::uint32_t typeMask =
      (0u | ... | detail::typeMask(Configs));
  inline static constexpr std::uint32_t typeValue =
      (0u | ... | detail::typeValue(Configs));
  inline static constexpr std::uint32_t speedMask =
      (0u | ... | detail::speedMask(Configs));
  inline static constexpr std::uint32_t speedValue =
      (0u | ... | detail::speedValue(Configs));
  inline static constexpr std::uint32_t pullMask =
      (0u | ... | detail::pullMask(Configs));
  inline static constexpr std::uint32_t pullValue =
      (0u | ... | detail::pullValue(Configs));

  static void apply() {
    if constexpr (modeMask != 0u)
      HwReg<std::uint32_t>::writeMasked(Port<Id>::modeReg(), modeMask,
                                        modeValue);

    if constexpr (typeMask != 0u)
      HwReg<std::uint32_t>::writeMasked(Port<Id>::typeReg(), typeMask,
                                        typeValue);

    if constexpr (speedMask != 0u)
      HwReg<std::uint32_t>::writeMasked(Port<Id>::speedReg(), speedMask,
                                        speedValue);

    if constexpr (pullMask != 0u)
      HwReg<std::uint32_t>::writeMasked(Port<Id>::pullReg(), pullMask,
                                        pullValue);
  }
};

/**
 * @brief Enables an OR-combined GPIO clock mask in one RCC update.
 * @tparam ClockMask Compile-time RCC AHB2 enable mask.
 */
template <std::uint32_t ClockMask>
struct GpioClockConfiguration {
  inline static constexpr std::uint32_t mask = ClockMask;

  static void apply() {
    if constexpr (mask != 0u) RCC<>::enableMask(mask);
  }
};

}  // namespace csl

#endif /* CSL23_GPIO_CONFIG_H_ */
