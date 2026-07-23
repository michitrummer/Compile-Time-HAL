/**
 * @file main.c
 * @brief Standalone application entry point for the NoHAL implementation.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "direct_gpio.h"

int main(void)
{
  const direct_port_config portB = {
    DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_MODE_OUTPUT),
    DIRECT_PIN_MASK(13u), DIRECT_PIN_VALUE(13u, GPIO_OUTPUT_PP),
    DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_SPEED_LOW),
    DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_NOPULL)
  };
  const direct_port_config portC = {
    DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_MODE_INPUT),
    0u, 0u, 0u, 0u,
    DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_NOPULL)
  };

  direct_enable_ports((1u << 1) | (1u << 2));
  direct_apply_port_config(GPIOB_BASE, &portB);
  direct_apply_port_config(GPIOC_BASE, &portC);

  while (1)
  {
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
    if (direct_get(GPIOC_BASE, 13u)) direct_set(GPIOB_BASE, 13u);
    else direct_clear(GPIOB_BASE, 13u);
  }
}
