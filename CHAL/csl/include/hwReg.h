/**
 * @file hwReg.h
 * @brief Inline helpers for 32-bit memory-mapped register operations.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_HWREG_H_
#define CSL_HWREG_H_

#include <stdbool.h>
#include <stdint.h>

/** @brief Sets selected bits in a 32-bit memory-mapped register. */
static inline void csl_hwReg32SetBits(volatile uint32_t* reg, uint32_t bits) {
  *reg |= bits;
}

/** @brief Clears selected bits in a 32-bit memory-mapped register. */
static inline void csl_hwReg32ClearBits(volatile uint32_t* reg, uint32_t bits) {
  *reg &= ~bits;
}

/**
 * @brief Tests whether all selected bits are set.
 * @return true when every bit in @p bits is set in @p reg.
 */
static inline bool csl_hwReg32AreBitsSet(const volatile uint32_t* reg,
                                         uint32_t bits) {
  return (*reg & bits) == bits;
}

/**
 * @brief Replaces a two-bit field in a 32-bit memory-mapped register.
 * @param reg Register to modify.
 * @param value Two-bit value to write.
 * @param shift Bit position of the field.
 */
static inline void csl_hwReg32Set2BitSubValue(volatile uint32_t* reg,
                                              uint32_t value,
                                              unsigned int shift) {
  csl_hwReg32ClearBits(reg, (uint32_t)((1u << 2u) - 1u) << shift);
  csl_hwReg32SetBits(reg, value << shift);
}

#endif /* CSL_HWREG_H_ */
