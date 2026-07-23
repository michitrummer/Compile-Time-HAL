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
#include <csl/include/Pin.h>

#include "./BindOnly.h"
namespace bsl {
/** @brief Board-support switch object with runtime-selected board mapping. */
class Switch {
 public:
  /** @brief Logical switch identifiers used by the board and benchmark
   * scenarios. */
  enum class Id { b1, ex1, ex2, ex3, ex4, ex5, ex6, ex7, ex8 };
  /** @brief Constructs and configures the selected switch GPIO. */
  Switch(Id id)
      : pin(portFor(id), pinFor(id), csl::Pin::inp, pullFor(id),
            csl::Pin::freqLow) {}
  /** @brief Binds the selected switch after aggregated board initialization. */
  Switch(Id id, BindOnly) : pin(portFor(id), pinFor(id)) {}
  /** @brief Reads the switch GPIO input. */
  bool pressed() const { return pin.get(); }

 private:
  static csl::Port::Id portFor(Id id) {
    switch (id) {
      case Id::b1:
      case Id::ex5:
      case Id::ex6:
        return csl::Port::c;
      case Id::ex7:
        return csl::Port::d;
      case Id::ex8:
        return csl::Port::e;
      default:
        return csl::Port::a;
    }
    return csl::Port::c;
  }
  static csl::Pin::Id pinFor(Id id) {
    switch (id) {
      case Id::b1:
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
        return csl::Pin::pin7;
    }
    return csl::Pin::pin13;
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
  csl::Pin pin;
};
}  // namespace bsl
#endif
