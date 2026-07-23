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
#include <csl/include/Pin.h>

#include <cstdint>

namespace bsl {

/** @brief Logical LED identifiers used by the board and benchmark scenarios. */
enum class LedId { ld4, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };

/**
 * @brief Maps a logical LED ID to a C++23 GPIO type and PinConfig value.
 * @tparam Id Logical board identifier.
 */
template <LedId Id>
struct LedTraits;

#define BSL23_LED_TRAITS(ID, PIN, PORT, MODE, PULL, SPEED, CLOCK)        \
  template <>                                                            \
  struct LedTraits<LedId::ID> {                                          \
    inline static constexpr csl::port::Id portId = csl::port::Id::PORT;  \
    inline static constexpr csl::PinConfig configuration{                \
        csl::pin::Id::PIN, csl::pin::Mode::MODE, csl::pin::Pull::PULL,   \
        csl::pin::Speed::SPEED};                                         \
    inline static constexpr std::uint32_t clockMask = csl::RCC<>::CLOCK; \
    using Pin = csl::Pin<configuration.id, portId>;                      \
  };

BSL23_LED_TRAITS(ld4, pin13, b, outPP, noPull, freqLow, ahb2GpioBEn)
BSL23_LED_TRAITS(ex1, pin0, b, outPP, pullUp, freqMid, ahb2GpioBEn)
BSL23_LED_TRAITS(ex2, pin1, b, outOD, pullDown, freqHigh, ahb2GpioBEn)
BSL23_LED_TRAITS(ex3, pin2, b, outPP, noPull, freqVeryHigh, ahb2GpioBEn)
BSL23_LED_TRAITS(ex4, pin3, b, outOD, pullUp, freqLow, ahb2GpioBEn)
BSL23_LED_TRAITS(ex5, pin4, b, outPP, pullDown, freqMid, ahb2GpioBEn)
BSL23_LED_TRAITS(ex6, pin5, b, outOD, noPull, freqHigh, ahb2GpioBEn)
BSL23_LED_TRAITS(ex7, pin6, e, outPP, pullUp, freqVeryHigh, ahb2GpioEEn)
BSL23_LED_TRAITS(ex8, pin0, h, outOD, pullDown, freqLow, ahb2GpioHEn)
#undef BSL23_LED_TRAITS

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
