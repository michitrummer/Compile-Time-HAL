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

#include "./bits.h"

namespace csl {

/**
 * @brief Provides stateless helpers for memory-mapped hardware registers.
 *
 * @tparam RegType Unsigned integer type matching the hardware register width.
 */
template <typename RegType>
class HwReg {
 public:
  /**
   * @brief Sets the specified bits in the register.
   *
   * @param reg register
   * @param bits bits to set
   */
  static void setBits(volatile RegType& reg, const RegType& bits) {
    reg |= bits;
  }

  /**
   * @brief Clears the specified bits in the register.
   *
   * @param reg register
   * @param bits bits to clear
   */
  static void clearBits(volatile RegType& reg, const RegType& bits) {
    reg &= ~bits;
  }

  /**
   * @brief Checks whether all selected bits are set in the register.
   *
   * @param reg register
   * @param bits bits to check
   *
   * @return true if the bits are set and false otherwise
   */
  static bool areBitsSet(const volatile RegType& reg, const RegType& bits) {
    return (reg & bits) == bits;
  }

  /**
   * @brief Replaces a fixed-width bit field in the register.
   *
   * @tparam bitWidth bit width of the sub-value
   *
   * @param reg register
   * @param value sub-value to set
   * @param shift shift for the sub-value
   */
  template <unsigned int bitWidth>
  static void setSubValue(volatile RegType& reg, const RegType& value,
                          unsigned int shift) {
    clearBits(reg,
              ((static_cast<RegType>(1) << bitWidth) - static_cast<RegType>(1))
                  << shift);
    setBits(reg, value << shift);
  }
};

}  // namespace csl

#endif /* CSL_HWREG_H_ */
