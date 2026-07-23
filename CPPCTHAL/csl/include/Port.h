/**
 * @file Port.h
 * @brief GPIO port register access and aggregated port configuration.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_PORT_H_
#define CSL_PORT_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "./periMemMap.h"

namespace csl {

namespace port {

enum Id : uint32_t {
  a = 0x0000ul,
  b = 0x0400ul,
  c = 0x0800ul,
  d = 0x0C00ul,
  e = 0x1000ul,
  h = 0x1C00ul
};

}  // namespace port

/**
 * @brief Stateless access to an STM32L433 GPIO port.
 *
 * The port address is part of the type. No port object or runtime pointer is
 * required. Hardware initialization is intentionally handled separately by
 * GpioConfig.h so several pins can be configured with one register update.
 */
template <port::Id id>
class Port {
 public:
  static volatile uint32_t& modeReg() { return registers().moder; }
  static volatile uint32_t& typeReg() { return registers().otyper; }
  static volatile uint32_t& speedReg() { return registers().ospeedr; }
  static volatile uint32_t& pullReg() { return registers().pupdr; }

  static const volatile uint32_t& inReg() { return registers().idr; }
  static const volatile uint32_t& outReg() { return registers().odr; }

  static volatile uint32_t& setReg() { return registers().bsrr; }
  static volatile uint32_t& rstReg() { return registers().brr; }

 private:
  struct Regs {
    volatile uint32_t moder;
    volatile uint32_t otyper;
    volatile uint32_t ospeedr;
    volatile uint32_t pupdr;
    volatile uint32_t idr;
    volatile uint32_t odr;
    volatile uint32_t bsrr;
    volatile uint32_t lckr;
    volatile uint32_t afr[2];
    volatile uint32_t brr;
  };

  static Regs& registers() {
    return *reinterpret_cast<Regs*>(static_cast<uint32_t>(basePeriphAHB2) + id);
  }

  static_assert(std::is_standard_layout<Regs>::value,
                "GPIO register block must be standard-layout");
  static_assert(sizeof(Regs) == 0x2Cu, "Unexpected GPIO register block size");
  static_assert(offsetof(Regs, moder) == 0x00u, "Unexpected GPIO MODER offset");
  static_assert(offsetof(Regs, otyper) == 0x04u,
                "Unexpected GPIO OTYPER offset");
  static_assert(offsetof(Regs, ospeedr) == 0x08u,
                "Unexpected GPIO OSPEEDR offset");
  static_assert(offsetof(Regs, pupdr) == 0x0Cu, "Unexpected GPIO PUPDR offset");
  static_assert(offsetof(Regs, idr) == 0x10u, "Unexpected GPIO IDR offset");
  static_assert(offsetof(Regs, odr) == 0x14u, "Unexpected GPIO ODR offset");
  static_assert(offsetof(Regs, bsrr) == 0x18u, "Unexpected GPIO BSRR offset");
  static_assert(offsetof(Regs, lckr) == 0x1Cu, "Unexpected GPIO LCKR offset");
  static_assert(offsetof(Regs, afr) == 0x20u, "Unexpected GPIO AFR offset");
  static_assert(offsetof(Regs, brr) == 0x28u, "Unexpected GPIO BRR offset");
};

}  // namespace csl

#endif /* CSL_PORT_H_ */
