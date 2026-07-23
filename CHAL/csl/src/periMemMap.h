/**
 * @file periMemMap.h
 * @brief STM32L433 peripheral memory-map constants.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_PERIMEMMAP_H_
#define CSL_PERIMEMMAP_H_

/**
 * @brief Peripheral memory map.
 */
enum {
  baseFlash = 0x08000000ul,  ///< up to 256KB
  baseSRAM1 = 0x20000000ul,  ///< up to 48KB
  baseSRAM2 = 0x10000000ul,  ///< up to 16KB
  basePeriph = 0x40000000ul,
  basePeriphAPB1 = basePeriph + 0x00000000ul,
  basePeriphAPB2 = basePeriph + 0x00010000ul,
  basePeriphAHB1 = basePeriph + 0x00020000ul,
  basePeriphAHB2 = basePeriph + 0x08000000ul
};

#endif /* CSL_PERIMEMMAP_H_ */
