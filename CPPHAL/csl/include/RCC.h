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

#ifndef CSL_RCC_H_
#define CSL_RCC_H_

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "./bits.h"

namespace csl
{

/** @brief Static reset-and-clock-control access for GPIO clock gating. */
class RCC
{
  public:
    /** @brief AHB2 peripheral clock-enable masks used by the benchmark. */
    enum PeriId : uint32_t
    {
      ahb2GpioAEn = bit0,
      ahb2GpioBEn = bit1,
      ahb2GpioCEn = bit2,
      ahb2GpioDEn = bit3,
      ahb2GpioEEn = bit4,
      ahb2GpioHEn = bit7
    };

    RCC() = default;

    /** @brief Enables one AHB2 peripheral. */
    static void periEnable(PeriId id);

    /** @brief Enables multiple AHB2 peripherals with one register update. */
    static void periEnableMask(uint32_t mask);

  private:
    struct Regs
    {
      volatile uint32_t cr;
      volatile uint32_t icscr;
      volatile uint32_t cfgr;
      volatile uint32_t pllcfgr;
      volatile uint32_t pllsai1cfgr;
      volatile uint32_t reserved0;
      volatile uint32_t cier;
      volatile uint32_t cifr;
      volatile uint32_t cicr;
      volatile uint32_t reserved1;
      volatile uint32_t ahb1rstr;
      volatile uint32_t ahb2rstr;
      volatile uint32_t ahb3rstr;
      volatile uint32_t reserved2;
      volatile uint32_t apb1rstr1;
      volatile uint32_t apb1rstr2;
      volatile uint32_t apb2rstr;
      volatile uint32_t reserved3;
      volatile uint32_t ahb1enr;
      volatile uint32_t ahb2enr;
      volatile uint32_t ahb3enr;
      volatile uint32_t reserved4;
      volatile uint32_t apb1enr1;
      volatile uint32_t apb1enr2;
      volatile uint32_t apb2enr;
      volatile uint32_t reserved5;
      volatile uint32_t ahb1smenr;
      volatile uint32_t ahb2smenr;
      volatile uint32_t ahb3smenr;
      volatile uint32_t reserved6;
      volatile uint32_t apb1smenr1;
      volatile uint32_t apb1smenr2;
      volatile uint32_t apb2smenr;
      volatile uint32_t reserved7;
      volatile uint32_t ccipr;
      volatile uint32_t reserved8;
      volatile uint32_t bdcr;
      volatile uint32_t csr;
      volatile uint32_t crrcr;
    };

    static_assert(std::is_standard_layout<Regs>::value,
                  "RCC register block must be standard-layout");
    static_assert(sizeof(Regs) == 0x9Cu, "Unexpected RCC register block size");
    static_assert(offsetof(Regs, cr) == 0x00u, "Unexpected RCC CR offset");
    static_assert(offsetof(Regs, ahb2enr) == 0x4Cu, "Unexpected RCC AHB2ENR offset");
    static_assert(offsetof(Regs, crrcr) == 0x98u, "Unexpected RCC CRRCR offset");

    enum : uint32_t { rccBaseOffset = 0x1000ul };
    static Regs& reg;
};

} // namespace csl

#endif /* CSL_RCC_H_ */
