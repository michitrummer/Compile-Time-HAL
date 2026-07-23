/**
 * @file switch.h
 * @brief Board-support switch abstraction for the direct-register baseline.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef NOHAL_BSL_SWITCH_H
#define NOHAL_BSL_SWITCH_H

#include <stdint.h>

#include "../../../InstructionAnalysis/support/direct_gpio.h"

/** @brief Logical switch identifiers used by the benchmark applications. */
typedef enum
{
  bsl_switchB1,
  bsl_switchEx1,
  bsl_switchEx2,
  bsl_switchEx3,
  bsl_switchEx4,
  bsl_switchEx5,
  bsl_switchEx6,
  bsl_switchEx7,
  bsl_switchEx8
} bsl_Switch;

/**
 * @brief Returns the GPIO base address assigned to a logical switch.
 * @param id Logical switch identifier.
 * @return STM32 GPIO peripheral base address.
 */
static inline uint32_t bsl_switchPort(bsl_Switch id)
{
  if (id == bsl_switchB1 || id == bsl_switchEx5 || id == bsl_switchEx6)
    return GPIOC_BASE;
  if (id == bsl_switchEx7)
    return GPIOD_BASE;
  if (id == bsl_switchEx8)
    return GPIOE_BASE;
  return GPIOA_BASE;
}

/**
 * @brief Returns the GPIO pin number assigned to a logical switch.
 * @param id Logical switch identifier.
 * @return GPIO pin number in the range 0..15.
 */
static inline uint32_t bsl_switchPin(bsl_Switch id)
{
  static const uint8_t pins[] = {13u, 0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u};
  return pins[(unsigned)id];
}

/**
 * @brief Reads the selected switch input.
 * @param id Logical switch identifier.
 * @retval 0 Input is low.
 * @retval 1 Input is high.
 */
static inline int bsl_switchPressed(bsl_Switch id)
{
  return direct_get(bsl_switchPort(id), bsl_switchPin(id));
}

#endif /* NOHAL_BSL_SWITCH_H */
