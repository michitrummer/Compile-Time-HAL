/**
 * @file led.h
 * @brief Board-support LED abstraction for the direct-register baseline.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef NOHAL_BSL_LED_H
#define NOHAL_BSL_LED_H

#include <stdint.h>

#include "../../../InstructionAnalysis/support/direct_gpio.h"

/** @brief Logical LED identifiers used by the benchmark applications. */
typedef enum {
  bsl_led4,
  bsl_ledEx1,
  bsl_ledEx2,
  bsl_ledEx3,
  bsl_ledEx4,
  bsl_ledEx5,
  bsl_ledEx6,
  bsl_ledEx7,
  bsl_ledEx8
} bsl_Led;

/**
 * @brief Returns the GPIO base address assigned to a logical LED.
 * @param id Logical LED identifier.
 * @return STM32 GPIO peripheral base address.
 */
static inline uint32_t bsl_ledPort(bsl_Led id) {
  return id == bsl_ledEx7   ? GPIOE_BASE
         : id == bsl_ledEx8 ? GPIOH_BASE
                            : GPIOB_BASE;
}

/**
 * @brief Returns the GPIO pin number assigned to a logical LED.
 * @param id Logical LED identifier.
 * @return GPIO pin number in the range 0..15.
 */
static inline uint32_t bsl_ledPin(bsl_Led id) {
  static const uint8_t pins[] = {13u, 0u, 1u, 2u, 3u, 4u, 5u, 6u, 0u};
  return pins[(unsigned)id];
}

/** @brief Drives the selected LED output high. */
static inline void bsl_ledOn(bsl_Led id) {
  direct_set(bsl_ledPort(id), bsl_ledPin(id));
}

/** @brief Drives the selected LED output low. */
static inline void bsl_ledOff(bsl_Led id) {
  direct_clear(bsl_ledPort(id), bsl_ledPin(id));
}

#endif /* NOHAL_BSL_LED_H */
