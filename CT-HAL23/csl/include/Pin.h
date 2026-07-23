/**
 * @file Pin.h
 * @brief GPIO pin abstraction for the C++ HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL23_PIN_H_
#define CSL23_PIN_H_

#include <cstdint>
#include <utility>

#include "./bits.h"
#include "./HwReg.h"
#include "./Port.h"

namespace csl
{
namespace pin
{

/** @brief STM32L433 GPIO pin numbers. */
enum class Id : std::uint32_t
{
  pin0 = 0, pin1, pin2, pin3, pin4, pin5, pin6, pin7,
  pin8, pin9, pin10, pin11, pin12, pin13, pin14, pin15
};

/** @brief GPIO operating modes supported by the benchmark HAL. */
enum class Mode : std::uint32_t
{
  inp   = 0x00000000u,
  outPP = 0x00000001u,
  outOD = 0x00000011u
};

/** @brief GPIO pull resistor configuration. */
enum class Pull : std::uint32_t
{
  noPull   = 0x00000000u,
  pullUp   = 0x00000001u,
  pullDown = 0x00000002u
};

/** @brief GPIO output speed encoding. */
enum class Speed : std::uint32_t
{
  freqLow      = 0x00000000u,
  freqMid      = 0x00000001u,
  freqHigh     = 0x00000002u,
  freqVeryHigh = 0x00000003u
};

} // namespace pin

/**
 * @brief Stateless GPIO access with pin and port encoded in the type.
 * @tparam PinId GPIO pin selected at compile time.
 * @tparam PortId Supported GPIO port selected at compile time.
 */
template<pin::Id PinId, port::Id PortId>
requires SupportedPort<PortId>
class Pin
{
  public:
    Pin() = default;

    /** @brief Sets the GPIO output through BSRR. */
    static void set()
    {
      HwReg<std::uint32_t>::setValue(Port<PortId>::setReg(), bit);
    }

    /** @brief Clears the GPIO output through BRR. */
    static void clear()
    {
      HwReg<std::uint32_t>::setValue(Port<PortId>::rstReg(), bit);
    }

    /** @brief Toggles the GPIO output based on the current ODR state. */
    static void toggle()
    {
      if (HwReg<std::uint32_t>::areBitsSet(Port<PortId>::outReg(), bit))
        clear();
      else
        set();
    }

    /** @brief Reads the GPIO input state. */
    static bool get()
    {
      return HwReg<std::uint32_t>::areBitsSet(Port<PortId>::inReg(), bit);
    }

  private:
    inline static constexpr std::uint32_t bit =
        bit0 << std::to_underlying(PinId);
};

} // namespace csl

#endif /* CSL23_PIN_H_ */
