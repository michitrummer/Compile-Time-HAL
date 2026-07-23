/**
 * @file rcc.h
 * @brief Reset and clock control abstraction for the C HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_RCC_H_
#define CSL_RCC_H_

#include <stdint.h>

#include "./bits.h"

/** @brief AHB2 peripheral-enable masks used by the GPIO benchmark. */
typedef enum {
  csl_ahb2GpioAEn = csl_bit0,
  csl_ahb2GpioBEn = csl_bit1,
  csl_ahb2GpioCEn = csl_bit2,
  csl_ahb2GpioDEn = csl_bit3,
  csl_ahb2GpioEEn = csl_bit4,
  csl_ahb2GpioHEn = csl_bit7
} csl_PeriId;

/** @brief Initializes RCC software state used by the compatibility path. */
void csl_rccInit(void);

/**
 * @brief Enables one AHB2 peripheral.
 * @param id Peripheral-enable mask.
 */
void csl_periEnable(csl_PeriId id);

/**
 * @brief Enables multiple AHB2 peripherals with one RCC_AHB2ENR update.
 * @param mask OR-combined peripheral-enable bits.
 */
void csl_periEnableMask(uint32_t mask);

#endif /* CSL_RCC_H_ */
