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

namespace csl
{

/**
 * @brief Runtime view of one STM32L433 GPIO register block.
 *
 * Port binding is separated from initialization so the benchmark can enable
 * clocks once and apply aggregated register masks per port.
 */
class Port
{
  public:
    /** @brief GPIO port identifiers encoded as AHB2 offsets. */
    enum Id : uint32_t
    {
      a = 0x0000ul,
      b = 0x0400ul,
      c = 0x0800ul,
      d = 0x0C00ul,
      e = 0x1000ul,
      h = 0x1C00ul
    };

    /**
     * @brief Aggregated register masks and values for one GPIO port.
     *
     * A zero mask leaves the corresponding register unchanged.
     */
    struct Configuration
    {
      uint32_t modeMask;
      uint32_t modeValue;
      uint32_t typeMask;
      uint32_t typeValue;
      uint32_t speedMask;
      uint32_t speedValue;
      uint32_t pullMask;
      uint32_t pullValue;
    };

    /**
     * @brief Binds a port register block without enabling its clock.
     * @param id GPIO port identifier.
     */
    explicit Port(Id id);

    /** @brief Enables all GPIO clocks selected by @p mask in one RCC update. */
    static void enableClocks(uint32_t mask);

    /** @brief Returns the RCC AHB2 enable mask associated with a GPIO port. */
    static uint32_t clockMask(Id id);

    /** @brief Applies an aggregated configuration to one GPIO port. */
    static void configure(Id id, const Configuration& config);

    volatile uint32_t& modeReg()  { return reg.moder; }
    volatile uint32_t& typeReg()  { return reg.otyper; }
    volatile uint32_t& speedReg() { return reg.ospeedr; }
    volatile uint32_t& pullReg()  { return reg.pupdr; }
    const volatile uint32_t& inReg() const  { return reg.idr; }
    const volatile uint32_t& outReg() const { return reg.odr; }
    volatile uint32_t& setReg() { return reg.bsrr; }
    volatile uint32_t& rstReg() { return reg.brr; }

  private:
    struct Regs
    {
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

    static_assert(std::is_standard_layout<Regs>::value,
                  "GPIO register block must be standard-layout");
    static_assert(sizeof(Regs) == 0x2Cu, "Unexpected GPIO register block size");
    static_assert(offsetof(Regs, moder) == 0x00u, "Unexpected GPIO MODER offset");
    static_assert(offsetof(Regs, otyper) == 0x04u, "Unexpected GPIO OTYPER offset");
    static_assert(offsetof(Regs, ospeedr) == 0x08u, "Unexpected GPIO OSPEEDR offset");
    static_assert(offsetof(Regs, pupdr) == 0x0Cu, "Unexpected GPIO PUPDR offset");
    static_assert(offsetof(Regs, idr) == 0x10u, "Unexpected GPIO IDR offset");
    static_assert(offsetof(Regs, odr) == 0x14u, "Unexpected GPIO ODR offset");
    static_assert(offsetof(Regs, bsrr) == 0x18u, "Unexpected GPIO BSRR offset");
    static_assert(offsetof(Regs, lckr) == 0x1Cu, "Unexpected GPIO LCKR offset");
    static_assert(offsetof(Regs, afr) == 0x20u, "Unexpected GPIO AFR offset");
    static_assert(offsetof(Regs, brr) == 0x28u, "Unexpected GPIO BRR offset");

    Regs& reg;
};

} // namespace csl

#endif /* CSL_PORT_H_ */
