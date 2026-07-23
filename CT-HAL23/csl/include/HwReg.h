/**
 * @file HwReg.h
 * @brief Helpers for type-safe memory-mapped register operations.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_HWREG_H_
#define CSL_HWREG_H_

#include <cstdint>

namespace csl
{

/**
 * @brief Stateless helper for accesses to memory-mapped hardware registers.
 *
 * @tparam RegType unsigned register type matching the register width
 */
template<typename RegType>
class HwReg
{
  public:
    static void setBits(volatile RegType& reg, RegType bits)
    {
      reg |= bits;
    }

    static void clearBits(volatile RegType& reg, RegType bits)
    {
      reg &= static_cast<RegType>(~bits);
    }

    static bool areBitsSet(const volatile RegType& reg, RegType bits)
    {
      return (reg & bits) == bits;
    }

    static void setValue(volatile RegType& reg, RegType value)
    {
      reg = value;
    }

    template<unsigned int bitWidth, unsigned int shift>
    static void setSubValue(volatile RegType& reg, RegType value)
    {
      const RegType mask =
          static_cast<RegType>(((static_cast<RegType>(1u) << bitWidth) - 1u) << shift);
      clearBits(reg, mask);
      setBits(reg, static_cast<RegType>(value << shift));
    }
};

} // namespace csl

#endif /* CSL_HWREG_H_ */
