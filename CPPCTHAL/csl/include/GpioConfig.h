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

#ifndef CSL_GPIO_CONFIG_H_
#define CSL_GPIO_CONFIG_H_

#include <cstdint>

#include "./Pin.h"
#include "./RCC.h"

namespace csl
{

/**
 * @brief Compile-time configuration of one GPIO pin.
 * @tparam id GPIO pin number.
 * @tparam mode Input or output mode.
 * @tparam pull Pull resistor configuration.
 * @tparam speed Output speed; ignored for input pins.
 *
 * All masks and values are constants and require no runtime storage.
 */
template<pin::Id id, pin::Mode mode, pin::Pull pull, pin::Speed speed = pin::freqLow>
struct PinConfiguration
{
  enum : uint32_t
  {
    pinBit = 1u << id,
    shift = static_cast<uint32_t>(id) * 2u,
    modeMask = 3u << shift,
    modeValue = (static_cast<uint32_t>(mode) & 3u) << shift,
    pullMask = 3u << shift,
    pullValue = static_cast<uint32_t>(pull) << shift,
    output = (mode == pin::outPP) || (mode == pin::outOD),
    typeMask = output ? pinBit : 0u,
    typeValue = (mode == pin::outOD) ? pinBit : 0u,
    speedMask = output ? (3u << shift) : 0u,
    speedValue = output ? (static_cast<uint32_t>(speed) << shift) : 0u
  };
};

namespace detail
{

template<bool... Values>
struct AllTrue;

template<>
struct AllTrue<>
{
  enum : bool { value = true };
};

template<bool Head, bool... Tail>
struct AllTrue<Head, Tail...>
{
  enum : bool { value = Head && AllTrue<Tail...>::value };
};

template<typename... Configs>
struct CombinedConfiguration;

template<>
struct CombinedConfiguration<>
{
  enum : uint32_t
  {
    modeMask = 0u, modeValue = 0u,
    typeMask = 0u, typeValue = 0u,
    speedMask = 0u, speedValue = 0u,
    pullMask = 0u, pullValue = 0u
  };
};

template<typename Head, typename... Tail>
struct CombinedConfiguration<Head, Tail...>
{
  typedef CombinedConfiguration<Tail...> Rest;
  static_assert((Head::modeMask & Rest::modeMask) == 0u,
                "A GPIO pin is configured more than once in one port configuration");
  enum : uint32_t
  {
    modeMask = Head::modeMask | Rest::modeMask,
    modeValue = Head::modeValue | Rest::modeValue,
    typeMask = Head::typeMask | Rest::typeMask,
    typeValue = Head::typeValue | Rest::typeValue,
    speedMask = Head::speedMask | Rest::speedMask,
    speedValue = Head::speedValue | Rest::speedValue,
    pullMask = Head::pullMask | Rest::pullMask,
    pullValue = Head::pullValue | Rest::pullValue
  };
};

} // namespace detail

/**
 * @brief Combines pin configurations and applies at most one update per register.
 * @tparam id GPIO port selected at compile time.
 * @tparam Traits GpioTraits types belonging to that port.
 *
 * CombinedConfiguration recursively folds masks and values at compile time and
 * rejects duplicate pins with a static assertion.
 */
template<port::Id id, typename... Traits>
struct PortConfiguration
{
  static void apply()
  {
    static_assert(detail::AllTrue<(Traits::portId == id)...>::value,
                  "PortConfiguration id must match every Traits::portId");

    typedef detail::CombinedConfiguration<typename Traits::Configuration...> Values;

    if (Values::modeMask != 0u)
      Port<id>::modeReg() = (Port<id>::modeReg() & ~Values::modeMask) | Values::modeValue;
    if (Values::typeMask != 0u)
      Port<id>::typeReg() = (Port<id>::typeReg() & ~Values::typeMask) | Values::typeValue;
    if (Values::speedMask != 0u)
      Port<id>::speedReg() = (Port<id>::speedReg() & ~Values::speedMask) | Values::speedValue;
    if (Values::pullMask != 0u)
      Port<id>::pullReg() = (Port<id>::pullReg() & ~Values::pullMask) | Values::pullValue;
  }
};

/**
 * @brief Enables an OR-combined GPIO clock mask in one RCC update.
 * @tparam clockMask Compile-time RCC AHB2 enable mask.
 */
template<uint32_t clockMask>
struct GpioClockConfiguration
{
  static void apply()
  {
    RCC<>::enableMask(clockMask);
  }
};

/**
 * @brief Shared compile-time GPIO mapping.
 * @tparam pinId Physical GPIO pin number.
 * @tparam portId Physical GPIO port.
 * @tparam mode Pin operating mode.
 * @tparam pull Pin pull resistor configuration.
 * @tparam speed Pin output speed (ignored for inputs).
 * @tparam clockMask RCC mask that enables the corresponding GPIO port clock.
 */
template<csl::pin::Id pinId,
         csl::port::Id port,
         csl::pin::Mode mode,
         csl::pin::Pull pull,
         csl::pin::Speed speed,
         uint32_t clockBits>
struct GpioTraits
{
  typedef csl::Pin<pinId, port> Pin;
  typedef csl::PinConfiguration<pinId, mode, pull, speed> Configuration;
  static constexpr csl::port::Id portId = port;
  static constexpr uint32_t clockMask = clockBits;
};

} // namespace csl

#endif /* CSL_GPIO_CONFIG_H_ */
