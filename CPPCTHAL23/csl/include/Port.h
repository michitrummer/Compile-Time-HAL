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

#ifndef CSL23_PORT_H_
#define CSL23_PORT_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "./periMemMap.h"

namespace csl {
namespace port {

enum class Id : std::uint32_t {
  a = 0x0000u,
  b = 0x0400u,
  c = 0x0800u,
  d = 0x0C00u,
  e = 0x1000u,
  h = 0x1C00u
};

}  // namespace port

template <port::Id Id>
concept SupportedPort =
    Id == port::Id::a || Id == port::Id::b || Id == port::Id::c ||
    Id == port::Id::d || Id == port::Id::e || Id == port::Id::h;

/**
 * @brief Stateless access to one compile-time-selected GPIO register block.
 * @tparam Id Supported STM32L433 GPIO port.
 */
template <port::Id Id>
  requires SupportedPort<Id>
class Port {
 public:
  static volatile std::uint32_t& modeReg() { return registers().moder; }
  static volatile std::uint32_t& typeReg() { return registers().otyper; }
  static volatile std::uint32_t& speedReg() { return registers().ospeedr; }
  static volatile std::uint32_t& pullReg() { return registers().pupdr; }

  static const volatile std::uint32_t& inReg() { return registers().idr; }
  static const volatile std::uint32_t& outReg() { return registers().odr; }

  static volatile std::uint32_t& setReg() { return registers().bsrr; }
  static volatile std::uint32_t& rstReg() { return registers().brr; }

 private:
  struct Regs {
    volatile std::uint32_t moder;
    volatile std::uint32_t otyper;
    volatile std::uint32_t ospeedr;
    volatile std::uint32_t pupdr;
    volatile std::uint32_t idr;
    volatile std::uint32_t odr;
    volatile std::uint32_t bsrr;
    volatile std::uint32_t lckr;
    volatile std::uint32_t afr[2];
    volatile std::uint32_t brr;
  };

  static Regs& registers() {
    constexpr auto address =
        static_cast<std::uint32_t>(basePeriphAHB2) + std::to_underlying(Id);
    return *reinterpret_cast<Regs*>(address);
  }

  static_assert(std::is_standard_layout_v<Regs>,
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

#endif /* CSL23_PORT_H_ */
