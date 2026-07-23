/**
 * @file Switch.h
 * @brief Board-support switch abstraction and board-to-GPIO mapping.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_SWITCH_H_
#define BSL_SWITCH_H_

#include <csl/include/GpioConfig.h>

namespace bsl {

/** @brief Logical switch identifiers used by the board and benchmark scenarios.
 */
enum class SwitchId { b1, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical switch ID to its compile-time GPIO type and
 * configuration.
 * @tparam id Logical board identifier.
 */
template <SwitchId id>
struct SwitchTraits;

template <>
struct SwitchTraits<SwitchId::b1>
    : csl::GpioTraits<csl::pin::pin13, csl::port::c, csl::pin::inp,
                      csl::pin::noPull, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex1>
    : csl::GpioTraits<csl::pin::pin0, csl::port::a, csl::pin::inp,
                      csl::pin::pullUp, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex2>
    : csl::GpioTraits<csl::pin::pin1, csl::port::a, csl::pin::inp,
                      csl::pin::pullDown, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex3>
    : csl::GpioTraits<csl::pin::pin2, csl::port::a, csl::pin::inp,
                      csl::pin::noPull, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex4>
    : csl::GpioTraits<csl::pin::pin3, csl::port::a, csl::pin::inp,
                      csl::pin::pullUp, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex5>
    : csl::GpioTraits<csl::pin::pin4, csl::port::c, csl::pin::inp,
                      csl::pin::pullDown, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex6>
    : csl::GpioTraits<csl::pin::pin5, csl::port::c, csl::pin::inp,
                      csl::pin::noPull, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex7>
    : csl::GpioTraits<csl::pin::pin6, csl::port::d, csl::pin::inp,
                      csl::pin::pullUp, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioDEn> {};

template <>
struct SwitchTraits<SwitchId::ex8>
    : csl::GpioTraits<csl::pin::pin7, csl::port::e, csl::pin::inp,
                      csl::pin::pullDown, csl::pin::freqLow,
                      csl::RCC<>::ahb2GpioEEn> {};

/**
 * @brief Zero-overhead board-support switch abstraction.
 * @tparam id Logical board identifier resolved to a GPIO at compile time.
 */
template <SwitchId id>
class Switch {
 public:
  Switch() = default;
  /** @brief Reads the switch GPIO input. */
  bool pressed() const { return pin.get(); }

 private:
  typedef SwitchTraits<id> Traits;
  typename Traits::Pin pin;
};

}  // namespace bsl
#endif
