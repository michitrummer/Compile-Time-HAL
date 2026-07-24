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

#ifndef BSL23_LED_H_
#define BSL23_LED_H_

#include <csl/include/GpioConfig.h>

namespace bsl {

/** @brief Logical LED identifiers used by the board and benchmark scenarios. */
enum class LedId { ld4, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical LED ID to its compile-time GPIO traits.
 * @tparam Id Logical board identifier.
 */
template <LedId Id>
struct LedTraits;

template <>
struct LedTraits<LedId::ld4>
    : csl::GpioTraits<csl::pin::Id::pin13, csl::port::Id::b,
                      csl::pin::Mode::outPP, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex1>
    : csl::GpioTraits<csl::pin::Id::pin0, csl::port::Id::b,
                      csl::pin::Mode::outPP, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqMid, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex2>
    : csl::GpioTraits<csl::pin::Id::pin1, csl::port::Id::b,
                      csl::pin::Mode::outOD, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqHigh, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex3>
    : csl::GpioTraits<csl::pin::Id::pin2, csl::port::Id::b,
                      csl::pin::Mode::outPP, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqVeryHigh,
                      csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex4>
    : csl::GpioTraits<csl::pin::Id::pin3, csl::port::Id::b,
                      csl::pin::Mode::outOD, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex5>
    : csl::GpioTraits<csl::pin::Id::pin4, csl::port::Id::b,
                      csl::pin::Mode::outPP, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqMid, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex6>
    : csl::GpioTraits<csl::pin::Id::pin5, csl::port::Id::b,
                      csl::pin::Mode::outOD, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqHigh, csl::RCC<>::ahb2GpioBEn> {};

template <>
struct LedTraits<LedId::ex7>
    : csl::GpioTraits<csl::pin::Id::pin6, csl::port::Id::e,
                      csl::pin::Mode::outPP, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqVeryHigh,
                      csl::RCC<>::ahb2GpioEEn> {};

template <>
struct LedTraits<LedId::ex8>
    : csl::GpioTraits<csl::pin::Id::pin0, csl::port::Id::h,
                      csl::pin::Mode::outOD, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioHEn> {};

/**
 * @brief Board-support LED abstraction resolved entirely at compile time.
 * @tparam Id Logical board identifier.
 */
template <LedId Id>
class Led {
 public:
  Led() = default;
  /** @brief Drives the LED output low. */
  void off() { pin.clear(); }
  /** @brief Drives the LED output high. */
  void on() { pin.set(); }

 private:
  using Traits = LedTraits<Id>;
  [[no_unique_address]] typename Traits::Pin pin{};
};

}  // namespace bsl
#endif
