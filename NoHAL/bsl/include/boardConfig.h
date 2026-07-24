/**
 * @file boardConfig.h
 * @brief Board-level GPIO initialization used by the benchmark applications.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef NOHAL_BSL_BOARD_CONFIG_H
#define NOHAL_BSL_BOARD_CONFIG_H
#include <Src/direct_gpio.h>
/** @brief Configures the standard LD4/B1 GPIO mapping with aggregated port
 * updates. */
static inline void bsl_boardConfigureGpio(void) {
  const direct_port_config b = {
      DIRECT_MODE_MASK(13u), DIRECT_MODE_VALUE(13u, GPIO_MODE_OUTPUT),
      DIRECT_PIN_MASK(13u),  0u,
      DIRECT_MODE_MASK(13u), 0u,
      DIRECT_MODE_MASK(13u), 0u};
  const direct_port_config c = {DIRECT_MODE_MASK(13u), 0u, 0u, 0u, 0u, 0u,
                                DIRECT_MODE_MASK(13u), 0u};
  direct_enable_ports((1u << 1) | (1u << 2));
  direct_apply_port_config(GPIOB_BASE, &b);
  direct_apply_port_config(GPIOC_BASE, &c);
}
/** @brief Configures all GPIOs used by the diversified MultiEx benchmark. */
static inline void bsl_boardConfigureGpioMultiEx(void) {
  const direct_port_config a = {0x000000FFu, 0x00000000u, 0u,         0u, 0u,
                                0u,          0x000000FFu, 0x00000049u};
  const direct_port_config b = {0x0C000FFFu, 0x04000555u, 0x0000203Fu,
                                0x0000002Au, 0x0C000FFFu, 0x00000939u,
                                0x0C000FFFu, 0x00000249u};
  const direct_port_config c = {0x0C000F00u, 0x00000000u, 0u,         0u, 0u,
                                0u,          0x0C000F00u, 0x00000200u};
  const direct_port_config d = {0x00003000u, 0x00000000u, 0u,         0u, 0u,
                                0u,          0x00003000u, 0x00001000u};
  const direct_port_config e = {0x0000F000u, 0x00001000u, 0x00000040u,
                                0u,          0x00003000u, 0x00003000u,
                                0x0000F000u, 0x00009000u};
  const direct_port_config h = {0x00000003u, 0x00000001u, 0x00000001u,
                                0x00000001u, 0x00000003u, 0u,
                                0x00000003u, 0x00000002u};
  direct_enable_ports((1u << 0) | (1u << 1) | (1u << 2) | (1u << 3) |
                      (1u << 4) | (1u << 7));
  direct_apply_port_config(GPIOA_BASE, &a);
  direct_apply_port_config(GPIOB_BASE, &b);
  direct_apply_port_config(GPIOC_BASE, &c);
  direct_apply_port_config(GPIOD_BASE, &d);
  direct_apply_port_config(GPIOE_BASE, &e);
  direct_apply_port_config(GPIOH_BASE, &h);
}
#endif
