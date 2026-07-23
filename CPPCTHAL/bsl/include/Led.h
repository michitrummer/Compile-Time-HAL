/**
 * @file Led.h
 * @brief Board-support LED abstraction and board-to-GPIO mapping.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_LED_H_
#define BSL_LED_H_

#include <cstdint>
#include <csl/include/GpioConfig.h>
#include <csl/include/Pin.h>

namespace bsl
{

/** @brief Logical LED identifiers used by the board and benchmark scenarios. */
enum class LedId
{
  ld4,
  ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8
};

/**
 * @brief Maps a logical LED ID to its compile-time GPIO type and configuration.
 * @tparam id Logical board identifier.
 */
template<LedId id>
struct LedTraits;

#define BSL_LED_TRAITS(ID, PIN, PORT, MODE, PULL, SPEED, CLOCK) \
template<> struct LedTraits<LedId::ID> { \
  typedef csl::Pin<csl::pin::PIN, csl::port::PORT> Pin; \
  typedef csl::PinConfiguration<csl::pin::PIN, csl::pin::MODE, csl::pin::PULL, csl::pin::SPEED> Configuration; \
  static constexpr csl::port::Id portId = csl::port::PORT; \
  static constexpr uint32_t clockMask = csl::RCC<>::CLOCK; \
};

BSL_LED_TRAITS(ld4, pin13, b, outPP, noPull,   freqLow,      ahb2GpioBEn)
BSL_LED_TRAITS(ex1, pin0,  b, outPP, pullUp,   freqMid,      ahb2GpioBEn)
BSL_LED_TRAITS(ex2, pin1,  b, outOD, pullDown, freqHigh,     ahb2GpioBEn)
BSL_LED_TRAITS(ex3, pin2,  b, outPP, noPull,   freqVeryHigh, ahb2GpioBEn)
BSL_LED_TRAITS(ex4, pin3,  b, outOD, pullUp,   freqLow,      ahb2GpioBEn)
BSL_LED_TRAITS(ex5, pin4,  b, outPP, pullDown, freqMid,      ahb2GpioBEn)
BSL_LED_TRAITS(ex6, pin5,  b, outOD, noPull,   freqHigh,     ahb2GpioBEn)
BSL_LED_TRAITS(ex7, pin6,  e, outPP, pullUp,   freqVeryHigh, ahb2GpioEEn)
BSL_LED_TRAITS(ex8, pin0,  h, outOD, pullDown, freqLow,      ahb2GpioHEn)

#undef BSL_LED_TRAITS

/**
 * @brief Zero-overhead board-support LED abstraction.
 * @tparam id Logical board identifier resolved to a GPIO at compile time.
 */
template<LedId id>
class Led
{
  public:
    Led() = default;
    /** @brief Drives the LED output low. */
    void off() { pin.clear(); }
    /** @brief Drives the LED output high. */
    void on()  { pin.set(); }

  private:
    typedef LedTraits<id> Traits;
    typename Traits::Pin pin;
};

} // namespace bsl
#endif
