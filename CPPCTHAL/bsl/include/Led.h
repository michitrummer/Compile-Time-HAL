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

#include <csl/include/GpioConfig.h>

namespace bsl {

/** @brief Logical LED identifiers used by the board and benchmark scenarios. */
enum class LedId { ld4, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical LED ID to its compile-time GPIO type and configuration.
 * @tparam id Logical board identifier.
 */
template <LedId id>
struct LedTraits;

template <>
struct LedTraits<LedId::ld4>
    : csl::GpioTraits<csl::pin::pin13, csl::port::b, csl::pin::outPP,
                      csl::pin::noPull, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex1>
    : csl::GpioTraits<csl::pin::pin0, csl::port::b, csl::pin::outPP,
                      csl::pin::pullUp, csl::pin::freqMid,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex2>
    : csl::GpioTraits<csl::pin::pin1, csl::port::b, csl::pin::outOD,
                      csl::pin::pullDown, csl::pin::freqHigh,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex3>
    : csl::GpioTraits<csl::pin::pin2, csl::port::b, csl::pin::outPP,
                      csl::pin::noPull, csl::pin::freqVeryHigh,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex4>
    : csl::GpioTraits<csl::pin::pin3, csl::port::b, csl::pin::outOD,
                      csl::pin::pullUp, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex5>
    : csl::GpioTraits<csl::pin::pin4, csl::port::b, csl::pin::outPP,
                      csl::pin::pullDown, csl::pin::freqMid,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex6>
    : csl::GpioTraits<csl::pin::pin5, csl::port::b, csl::pin::outOD,
                      csl::pin::noPull, csl::pin::freqHigh,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex7>
    : csl::GpioTraits<csl::pin::pin6, csl::port::e, csl::pin::outPP,
                      csl::pin::pullUp, csl::pin::freqVeryHigh,
                      csl::RCC<>::ahb2GpioEEn> {};

template <>
struct LedTraits<LedId::ex8>
    : csl::GpioTraits<csl::pin::pin0, csl::port::h, csl::pin::outOD,
                      csl::pin::pullDown, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioHEn> {};

/**
 * @brief Zero-overhead board-support LED abstraction.
 * @tparam id Logical board identifier resolved to a GPIO at compile time.
 */
template <LedId id>
class Led {
 public:
  Led() = default;
  /** @brief Drives the LED output low. */
  void off() { pin.clear(); }
  /** @brief Drives the LED output high. */
  void on() { pin.set(); }

 private:
  typedef LedTraits<id> Traits;
  typename Traits::Pin pin;
};

}  // namespace bsl
#endif
