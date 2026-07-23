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

#include <cstdint>
#include <csl/include/GpioConfig.h>
#include <csl/include/Pin.h>

namespace bsl
{

/** @brief Logical switch identifiers used by the board and benchmark scenarios. */
enum class SwitchId
{
  b1,
  ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8
};

/**
 * @brief Maps a logical switch ID to its compile-time GPIO type and configuration.
 * @tparam id Logical board identifier.
 */
template<SwitchId id>
struct SwitchTraits;

#define BSL_SWITCH_TRAITS(ID, PIN, PORT, PULL, CLOCK) \
template<> struct SwitchTraits<SwitchId::ID> { \
  typedef csl::Pin<csl::pin::PIN, csl::port::PORT> Pin; \
  typedef csl::PinConfiguration<csl::pin::PIN, csl::pin::inp, csl::pin::PULL, csl::pin::freqLow> Configuration; \
  static constexpr csl::port::Id portId = csl::port::PORT; \
  static constexpr uint32_t clockMask = csl::RCC<>::CLOCK; \
};

BSL_SWITCH_TRAITS(b1,  pin13, c, noPull,   ahb2GpioCEn)
BSL_SWITCH_TRAITS(ex1, pin0,  a, pullUp,   ahb2GpioAEn)
BSL_SWITCH_TRAITS(ex2, pin1,  a, pullDown, ahb2GpioAEn)
BSL_SWITCH_TRAITS(ex3, pin2,  a, noPull,   ahb2GpioAEn)
BSL_SWITCH_TRAITS(ex4, pin3,  a, pullUp,   ahb2GpioAEn)
BSL_SWITCH_TRAITS(ex5, pin4,  c, pullDown, ahb2GpioCEn)
BSL_SWITCH_TRAITS(ex6, pin5,  c, noPull,   ahb2GpioCEn)
BSL_SWITCH_TRAITS(ex7, pin6,  d, pullUp,   ahb2GpioDEn)
BSL_SWITCH_TRAITS(ex8, pin7,  e, pullDown, ahb2GpioEEn)

#undef BSL_SWITCH_TRAITS

/**
 * @brief Zero-overhead board-support switch abstraction.
 * @tparam id Logical board identifier resolved to a GPIO at compile time.
 */
template<SwitchId id>
class Switch
{
  public:
    Switch() = default;
    /** @brief Reads the switch GPIO input. */
    bool pressed() const { return pin.get(); }

  private:
    typedef SwitchTraits<id> Traits;
    typename Traits::Pin pin;
};

} // namespace bsl
#endif
