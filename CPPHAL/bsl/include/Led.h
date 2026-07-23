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
#include <csl/include/Pin.h>

#include "./BindOnly.h"
namespace bsl {
/** @brief Board-support LED object with runtime-selected board mapping. */
class Led {
 public:
  /** @brief Logical LED identifiers used by the board and benchmark scenarios.
   */
  enum class Id { ld4, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };
  /** @brief Constructs and configures the selected LED GPIO. */
  Led(Id id)
      : pin(portFor(id), pinFor(id), modeFor(id), pullFor(id), speedFor(id)) {}
  /** @brief Binds the selected LED after aggregated board initialization. */
  Led(Id id, BindOnly) : pin(portFor(id), pinFor(id)) {}
  /** @brief Drives the LED output low. */
  void off() { pin.clear(); }
  /** @brief Drives the LED output high. */
  void on() { pin.set(); }

 private:
  static csl::Port::Id portFor(Id id) {
    switch (id) {
      case Id::ex7:
        return csl::Port::e;
      case Id::ex8:
        return csl::Port::h;
      default:
        return csl::Port::b;
    }
    return csl::Port::b;
  }
  static csl::Pin::Id pinFor(Id id) {
    switch (id) {
      case Id::ld4:
        return csl::Pin::pin13;
      case Id::ex1:
        return csl::Pin::pin0;
      case Id::ex2:
        return csl::Pin::pin1;
      case Id::ex3:
        return csl::Pin::pin2;
      case Id::ex4:
        return csl::Pin::pin3;
      case Id::ex5:
        return csl::Pin::pin4;
      case Id::ex6:
        return csl::Pin::pin5;
      case Id::ex7:
        return csl::Pin::pin6;
      case Id::ex8:
        return csl::Pin::pin0;
    }
    return csl::Pin::pin13;
  }
  static csl::Pin::Mode modeFor(Id id) {
    return (id == Id::ex2 || id == Id::ex4 || id == Id::ex6 || id == Id::ex8)
               ? csl::Pin::outOD
               : csl::Pin::outPP;
  }
  static csl::Pin::Pull pullFor(Id id) {
    switch (id) {
      case Id::ex1:
      case Id::ex4:
      case Id::ex7:
        return csl::Pin::pullUp;
      case Id::ex2:
      case Id::ex5:
      case Id::ex8:
        return csl::Pin::pullDown;
      default:
        return csl::Pin::noPull;
    }
  }
  static csl::Pin::Speed speedFor(Id id) {
    switch (id) {
      case Id::ex1:
      case Id::ex5:
        return csl::Pin::freqMid;
      case Id::ex2:
      case Id::ex6:
        return csl::Pin::freqHigh;
      case Id::ex3:
      case Id::ex7:
        return csl::Pin::freqVeryHigh;
      default:
        return csl::Pin::freqLow;
    }
  }
  csl::Pin pin;
};
}  // namespace bsl
#endif
