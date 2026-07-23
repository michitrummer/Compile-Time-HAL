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

/**
 * @brief Runtime GPIO pin handle for the classic C++ HAL.
 *
 * The two-argument constructor only binds the memory-mapped GPIO. The
 * configuration constructor is retained as a compatibility path; aggregated
 * benchmark initialization is performed by BoardConfig before binding.
 */
class Pin
{
  public:
    /** @brief STM32L433 GPIO pin numbers. */
    enum Id : uint32_t
    {
      pin0 = 0, pin1, pin2, pin3, pin4, pin5, pin6, pin7,
      pin8, pin9, pin10, pin11, pin12, pin13, pin14, pin15
    };

    /** @brief GPIO operating modes supported by the benchmark HAL. */
    enum Mode : uint32_t
    {
      inp = 0x00000000u,
      outPP = 0x00000001u,
      outOD = 0x00000011u
    };

    /** @brief GPIO pull resistor configuration. */
    enum Pull : uint32_t
    {
      noPull = 0x00000000u,
      pullUp = 0x00000001u,
      pullDown = 0x00000002u
    };

    /** @brief GPIO output speed encoding. */
    enum Speed : uint32_t
    {
      freqLow = 0x00000000u,
      freqMid = 0x00000001u,
      freqHigh = 0x00000002u,
      freqVeryHigh = 0x00000003u
    };

    /**
     * @brief Binds a GPIO pin without changing hardware configuration.
     * @param portId GPIO port identifier.
     * @param id GPIO pin number.
     */
    Pin(Port::Id portId, Id id);

    /**
     * @brief Enables the port clock, binds, and configures one GPIO pin.
     * @param portId GPIO port identifier.
     * @param id GPIO pin number.
     * @param mod Input or output mode.
     * @param pull Pull resistor configuration.
     * @param speed Output speed; ignored for input mode.
     * @note The benchmark uses aggregated BoardConfig initialization instead.
     */
    Pin(Port::Id portId, Id id, Mode mod,
        Pull pull = noPull, Speed speed = freqLow);

    /** @brief Sets the GPIO output atomically through BSRR. */
    void set() { port.setReg() = bit; }
    /** @brief Clears the GPIO output atomically through BRR. */
    void clear() { port.rstReg() = bit; }
    /** @brief Toggles the GPIO output based on the current ODR state. */
    void toggle()
    {
      if (HwReg<uint32_t>::areBitsSet(port.outReg(), bit)) clear();
      else set();
    }
    /** @brief Reads the GPIO input state. */
    bool get() const
    {
      return HwReg<uint32_t>::areBitsSet(port.inReg(), bit);
    }

  private:
    enum { gpioOutputTypeMask = bit4, gpioOutputTypeShift = 4u };
    Port port;
    const uint32_t bit;
};

} // namespace csl

#endif /* CSL_PIN_H_ */
