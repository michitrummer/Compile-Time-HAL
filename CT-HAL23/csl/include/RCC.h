/**
 * @file RCC.h
 * @brief Reset and clock control abstraction for GPIO clock gating.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL23_RCC_H_
#define CSL23_RCC_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "./HwReg.h"
#include "./bits.h"
#include "./periMemMap.h"

namespace csl {
namespace rcc {

enum class Id : std::uint32_t { rcc = 0x1000u };

}  // namespace rcc

/**
 * @brief Static C++23 reset-and-clock-control access.
 * @tparam Base RCC base offset encoded as a strongly typed enum value.
 */
template <rcc::Id Base = rcc::Id::rcc>
class RCC {
 public:
  inline static constexpr std::uint32_t ahb2GpioAEn = bit0;
  inline static constexpr std::uint32_t ahb2GpioBEn = bit1;
  inline static constexpr std::uint32_t ahb2GpioCEn = bit2;
  inline static constexpr std::uint32_t ahb2GpioDEn = bit3;
  inline static constexpr std::uint32_t ahb2GpioEEn = bit4;
  inline static constexpr std::uint32_t ahb2GpioHEn = bit7;

  /** @brief Enables multiple AHB2 peripherals with one register update. */
  static void enableMask(std::uint32_t mask) {
    auto& regs = registers();
    HwReg<std::uint32_t>::setBits(regs.ahb2enr, mask);
    (void)regs.ahb2enr;
  }

 private:
  struct Regs {
    volatile std::uint32_t cr;
    volatile std::uint32_t icscr;
    volatile std::uint32_t cfgr;
    volatile std::uint32_t pllcfgr;
    volatile std::uint32_t pllsai1cfgr;
    volatile std::uint32_t reserved0;
    volatile std::uint32_t cier;
    volatile std::uint32_t cifr;
    volatile std::uint32_t cicr;
    volatile std::uint32_t reserved1;
    volatile std::uint32_t ahb1rstr;
    volatile std::uint32_t ahb2rstr;
    volatile std::uint32_t ahb3rstr;
    volatile std::uint32_t reserved2;
    volatile std::uint32_t apb1rstr1;
    volatile std::uint32_t apb1rstr2;
    volatile std::uint32_t apb2rstr;
    volatile std::uint32_t reserved3;
    volatile std::uint32_t ahb1enr;
    volatile std::uint32_t ahb2enr;
    volatile std::uint32_t ahb3enr;
    volatile std::uint32_t reserved4;
    volatile std::uint32_t apb1enr1;
    volatile std::uint32_t apb1enr2;
    volatile std::uint32_t apb2enr;
    volatile std::uint32_t reserved5;
    volatile std::uint32_t ahb1smenr;
    volatile std::uint32_t ahb2smenr;
    volatile std::uint32_t ahb3smenr;
    volatile std::uint32_t reserved6;
    volatile std::uint32_t apb1smenr1;
    volatile std::uint32_t apb1smenr2;
    volatile std::uint32_t apb2smenr;
    volatile std::uint32_t reserved7;
    volatile std::uint32_t ccipr;
    volatile std::uint32_t reserved8;
    volatile std::uint32_t bdcr;
    volatile std::uint32_t csr;
    volatile std::uint32_t crrcr;
  };

  static Regs& registers() {
    constexpr auto address =
        static_cast<std::uint32_t>(basePeriphAHB1) + std::to_underlying(Base);
    return *reinterpret_cast<Regs*>(address);
  }

  static_assert(std::is_standard_layout_v<Regs>,
                "RCC register block must be standard-layout");
  static_assert(sizeof(Regs) == 0x9Cu, "Unexpected RCC register block size");
  static_assert(offsetof(Regs, cr) == 0x00u, "Unexpected RCC CR offset");
  static_assert(offsetof(Regs, ahb2enr) == 0x4Cu,
                "Unexpected RCC AHB2ENR offset");
  static_assert(offsetof(Regs, crrcr) == 0x98u, "Unexpected RCC CRRCR offset");
};

}  // namespace csl

#endif /* CSL23_RCC_H_ */
