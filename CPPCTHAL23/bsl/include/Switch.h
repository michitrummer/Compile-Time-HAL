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

#ifndef BSL23_SWITCH_H_
#define BSL23_SWITCH_H_

#include <csl/include/GpioConfig.h>

namespace bsl {

/** @brief Logical switch identifiers used by the board and benchmark scenarios.
 */
enum class SwitchId { b1, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical switch ID to its compile-time GPIO traits.
 * @tparam Id Logical board identifier.
 */
template <SwitchId Id>
struct SwitchTraits;

template <>
struct SwitchTraits<SwitchId::b1>
    : csl::GpioTraits<csl::pin::Id::pin13, csl::port::Id::c,
                      csl::pin::Mode::inp, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex1>
    : csl::GpioTraits<csl::pin::Id::pin0, csl::port::Id::a,
                      csl::pin::Mode::inp, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex2>
    : csl::GpioTraits<csl::pin::Id::pin1, csl::port::Id::a,
                      csl::pin::Mode::inp, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex3>
    : csl::GpioTraits<csl::pin::Id::pin2, csl::port::Id::a,
                      csl::pin::Mode::inp, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex4>
    : csl::GpioTraits<csl::pin::Id::pin3, csl::port::Id::a,
                      csl::pin::Mode::inp, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioAEn> {};

template <>
struct SwitchTraits<SwitchId::ex5>
    : csl::GpioTraits<csl::pin::Id::pin4, csl::port::Id::c,
                      csl::pin::Mode::inp, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex6>
    : csl::GpioTraits<csl::pin::Id::pin5, csl::port::Id::c,
                      csl::pin::Mode::inp, csl::pin::Pull::noPull,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioCEn> {};

template <>
struct SwitchTraits<SwitchId::ex7>
    : csl::GpioTraits<csl::pin::Id::pin6, csl::port::Id::d,
                      csl::pin::Mode::inp, csl::pin::Pull::pullUp,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioDEn> {};

template <>
struct SwitchTraits<SwitchId::ex8>
    : csl::GpioTraits<csl::pin::Id::pin7, csl::port::Id::e,
                      csl::pin::Mode::inp, csl::pin::Pull::pullDown,
                      csl::pin::Speed::freqLow, csl::RCC<>::ahb2GpioEEn> {};

/**
 * @brief Board-support switch abstraction resolved entirely at compile time.
 * @tparam Id Logical board identifier.
 */
template <SwitchId Id>
class Switch {
 public:
  Switch() = default;
  /** @brief Reads the switch GPIO input. */
  bool pressed() const { return pin.get(); }

 private:
  using Traits = SwitchTraits<Id>;
  [[no_unique_address]] typename Traits::Pin pin{};
};

}  // namespace bsl
#endif
