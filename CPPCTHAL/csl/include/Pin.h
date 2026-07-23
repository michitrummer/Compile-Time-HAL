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

#ifndef CSL_PIN_H_
#define CSL_PIN_H_

#include <cstdint>

#include "./bits.h"
#include "./HwReg.h"
#include "./Port.h"

namespace csl
{

namespace pin
{

/** @brief STM32L433 GPIO pin numbers. */
enum Id : uint32_t
{
  pin0 = 0, pin1, pin2, pin3, pin4, pin5, pin6, pin7,
  pin8, pin9, pin10, pin11, pin12, pin13, pin14, pin15
};

/** @brief GPIO operating modes supported by the benchmark HAL. */
enum Mode : uint32_t
{
  inp   = 0x00000000u,
  outPP = 0x00000001u,
  outOD = 0x00000011u
};

/** @brief GPIO pull resistor configuration. */
enum Pull : uint32_t
{
  noPull   = 0x00000000u,
  pullUp   = 0x00000001u,
  pullDown = 0x00000002u
};

/** @brief GPIO output speed encoding. */
enum Speed : uint32_t
{
  freqLow      = 0x00000000u,
  freqMid      = 0x00000001u,
  freqHigh     = 0x00000002u,
  freqVeryHigh = 0x00000003u
};

} // namespace pin

/**
 * @brief Stateless compile-time GPIO access type.
 *
 * Port address and pin mask are compile-time constants. Pin configuration is
 * deliberately separated from access and applied by PortConfiguration so that
 * several pins of one port share one read/modify/write sequence per register.
 */
template<pin::Id pinId, port::Id portId>
class Pin
{
  public:
    Pin() = default;

    /** @brief Sets the compile-time-selected GPIO output through BSRR. */
    static void set()
    {
      HwReg<uint32_t>::setValue(Port<portId>::setReg(), bit);
    }

    /** @brief Clears the compile-time-selected GPIO output through BRR. */
    static void clear()
    {
      HwReg<uint32_t>::setValue(Port<portId>::rstReg(), bit);
    }

    /** @brief Toggles the GPIO output based on the current ODR state. */
    static void toggle()
    {
      if (HwReg<uint32_t>::areBitsSet(Port<portId>::outReg(), bit))
        clear();
      else
        set();
    }

    /** @brief Reads the compile-time-selected GPIO input. */
    static bool get()
    {
      return HwReg<uint32_t>::areBitsSet(Port<portId>::inReg(), bit);
    }

  private:
    enum : uint32_t { bit = bit0 << pinId };
};

} // namespace csl

#endif /* CSL_PIN_H_ */
