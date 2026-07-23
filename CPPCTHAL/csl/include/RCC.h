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
#include "./HwReg.h"
#include "./periMemMap.h"

namespace csl
{

namespace rcc
{

enum Id : uint32_t
{
  rcc = 0x1000ul
};

} // namespace rcc

/**
 * @brief Provides static STM32L433 reset-and-clock-control access.
 */
template<rcc::Id base = rcc::rcc>
class RCC
{
  public:
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
    static void periEnable(PeriId id)
    {
      enableMask(static_cast<uint32_t>(id));
    }

    /** @brief Enables multiple AHB2 peripherals with one register update. */
    static void enableMask(uint32_t mask)
    {
      Regs& regs = registers();
      HwReg<uint32_t>::setBits(regs.ahb2enr, mask);
      (void)regs.ahb2enr;
    }

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

    static Regs& registers()
    {
      return *reinterpret_cast<Regs*>(static_cast<uint32_t>(basePeriphAHB1) + base);
    }

    static_assert(std::is_standard_layout<Regs>::value,
                  "RCC register block must be standard-layout");
    static_assert(sizeof(Regs) == 0x9Cu, "Unexpected RCC register block size");
    static_assert(offsetof(Regs, cr) == 0x00u, "Unexpected RCC CR offset");
    static_assert(offsetof(Regs, ahb2enr) == 0x4Cu, "Unexpected RCC AHB2ENR offset");
    static_assert(offsetof(Regs, crrcr) == 0x98u, "Unexpected RCC CRRCR offset");
};

} // namespace csl

#endif /* CSL_RCC_H_ */
