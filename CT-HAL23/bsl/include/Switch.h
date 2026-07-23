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

#include <cstdint>
#include <csl/include/GpioConfig.h>
#include <csl/include/Pin.h>

namespace bsl
{

/** @brief Logical switch identifiers used by the board and benchmark scenarios. */
enum class SwitchId { b1, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical switch ID to a C++23 GPIO type and PinConfig value.
 * @tparam Id Logical board identifier.
 */
template<SwitchId Id> struct SwitchTraits;

#define BSL23_SWITCH_TRAITS(ID, PIN, PORT, PULL, CLOCK) \
template<> struct SwitchTraits<SwitchId::ID> { \
  inline static constexpr csl::port::Id portId = csl::port::Id::PORT; \
  inline static constexpr csl::PinConfig configuration{csl::pin::Id::PIN, csl::pin::Mode::inp, csl::pin::Pull::PULL, csl::pin::Speed::freqLow}; \
  inline static constexpr std::uint32_t clockMask = csl::RCC<>::CLOCK; \
  using Pin = csl::Pin<configuration.id, portId>; \
};

BSL23_SWITCH_TRAITS(b1,  pin13, c, noPull,   ahb2GpioCEn)
BSL23_SWITCH_TRAITS(ex1, pin0,  a, pullUp,   ahb2GpioAEn)
BSL23_SWITCH_TRAITS(ex2, pin1,  a, pullDown, ahb2GpioAEn)
BSL23_SWITCH_TRAITS(ex3, pin2,  a, noPull,   ahb2GpioAEn)
BSL23_SWITCH_TRAITS(ex4, pin3,  a, pullUp,   ahb2GpioAEn)
BSL23_SWITCH_TRAITS(ex5, pin4,  c, pullDown, ahb2GpioCEn)
BSL23_SWITCH_TRAITS(ex6, pin5,  c, noPull,   ahb2GpioCEn)
BSL23_SWITCH_TRAITS(ex7, pin6,  d, pullUp,   ahb2GpioDEn)
BSL23_SWITCH_TRAITS(ex8, pin7,  e, pullDown, ahb2GpioEEn)
#undef BSL23_SWITCH_TRAITS

/**
 * @brief Board-support switch abstraction resolved entirely at compile time.
 * @tparam Id Logical board identifier.
 */
template<SwitchId Id>
class Switch
{
  public:
    Switch() = default;
    /** @brief Reads the switch GPIO input. */
    bool pressed() const { return pin.get(); }
  private:
    using Traits = SwitchTraits<Id>;
    [[no_unique_address]] typename Traits::Pin pin{};
};

} // namespace bsl
#endif
