/**
 * @file direct_gpio.h
 * @brief Direct STM32L433 GPIO register access used as the benchmark baseline.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef INSTRUCTION_ANALYSIS_DIRECT_GPIO_H
#define INSTRUCTION_ANALYSIS_DIRECT_GPIO_H

#include <stdint.h>

#define PERIPH_BASE       0x40000000ul
#define AHB1PERIPH_BASE   (PERIPH_BASE + 0x00020000ul)
#define AHB2PERIPH_BASE   (PERIPH_BASE + 0x08000000ul)
#define RCC_BASE          (AHB1PERIPH_BASE + 0x1000ul)
#define RCC_AHB2ENR       (*(volatile uint32_t *)(RCC_BASE + 0x4Cul))

#define GPIOA_BASE        (AHB2PERIPH_BASE + 0x0000ul)
#define GPIOB_BASE        (AHB2PERIPH_BASE + 0x0400ul)
#define GPIOC_BASE        (AHB2PERIPH_BASE + 0x0800ul)
#define GPIOD_BASE        (AHB2PERIPH_BASE + 0x0C00ul)
#define GPIOE_BASE        (AHB2PERIPH_BASE + 0x1000ul)
#define GPIOH_BASE        (AHB2PERIPH_BASE + 0x1C00ul)

#define GPIO_MODER(port)   (*(volatile uint32_t *)((port) + 0x00ul))
#define GPIO_OTYPER(port)  (*(volatile uint32_t *)((port) + 0x04ul))
#define GPIO_OSPEEDR(port) (*(volatile uint32_t *)((port) + 0x08ul))
#define GPIO_PUPDR(port)   (*(volatile uint32_t *)((port) + 0x0Cul))
#define GPIO_IDR(port)     (*(volatile uint32_t *)((port) + 0x10ul))
#define GPIO_BSRR(port)    (*(volatile uint32_t *)((port) + 0x18ul))
#define GPIO_BRR(port)     (*(volatile uint32_t *)((port) + 0x28ul))

#define GPIO_PIN(n)        (1ul << (n))
#define GPIO_MODE_INPUT    0ul
#define GPIO_MODE_OUTPUT   1ul
#define GPIO_OUTPUT_PP     0ul
#define GPIO_OUTPUT_OD     1ul
#define GPIO_NOPULL        0ul
#define GPIO_PULLUP        1ul
#define GPIO_PULLDOWN      2ul
#define GPIO_SPEED_LOW     0ul
#define GPIO_SPEED_MEDIUM  1ul
#define GPIO_SPEED_HIGH    2ul
#define GPIO_SPEED_VHIGH   3ul

/**
 * @brief Enables one or more GPIO peripheral clocks with a single RCC update.
 * @param mask Bit mask for RCC_AHB2ENR.
 */
static inline void direct_enable_ports(uint32_t mask)
{
  RCC_AHB2ENR |= mask;
  const volatile uint32_t delay = RCC_AHB2ENR;
  (void)delay;
}

/**
 * @brief Configures one GPIO pin as an input.
 * @param port GPIO peripheral base address.
 * @param pin GPIO pin number in the range 0..15.
 * @param pull Pull configuration encoded for GPIO_PUPDR.
 */
static inline void direct_init_input(uint32_t port, uint32_t pin, uint32_t pull)
{
  const uint32_t shift = pin * 2u;
  GPIO_MODER(port) = (GPIO_MODER(port) & ~(3ul << shift)) |
                     (GPIO_MODE_INPUT << shift);
  GPIO_PUPDR(port) = (GPIO_PUPDR(port) & ~(3ul << shift)) |
                     (pull << shift);
}

/**
 * @brief Configures one GPIO pin as an output.
 * @param port GPIO peripheral base address.
 * @param pin GPIO pin number in the range 0..15.
 * @param output_type Push-pull or open-drain encoding for GPIO_OTYPER.
 * @param pull Pull configuration encoded for GPIO_PUPDR.
 * @param speed Speed configuration encoded for GPIO_OSPEEDR.
 */
static inline void direct_init_output(uint32_t port, uint32_t pin,
                                      uint32_t output_type, uint32_t pull,
                                      uint32_t speed)
{
  const uint32_t shift = pin * 2u;
  GPIO_MODER(port) = (GPIO_MODER(port) & ~(3ul << shift)) |
                     (GPIO_MODE_OUTPUT << shift);
  GPIO_OSPEEDR(port) = (GPIO_OSPEEDR(port) & ~(3ul << shift)) |
                       (speed << shift);
  GPIO_OTYPER(port) = (GPIO_OTYPER(port) & ~GPIO_PIN(pin)) |
                      (output_type << pin);
  GPIO_PUPDR(port) = (GPIO_PUPDR(port) & ~(3ul << shift)) |
                     (pull << shift);
}


/** @brief Aggregated masks and values for configuring one GPIO port. */
typedef struct
{
  uint32_t mode_mask;
  uint32_t mode_value;
  uint32_t type_mask;
  uint32_t type_value;
  uint32_t speed_mask;
  uint32_t speed_value;
  uint32_t pull_mask;
  uint32_t pull_value;
} direct_port_config;

/**
 * @brief Applies aggregated GPIO configuration with at most one RMW per register.
 * @param port GPIO peripheral base address.
 * @param config Aggregated register masks and values.
 */
static inline void direct_apply_port_config(uint32_t port, const direct_port_config* config)
{
  if (config->mode_mask != 0u)
    GPIO_MODER(port) = (GPIO_MODER(port) & ~config->mode_mask) | config->mode_value;
  if (config->type_mask != 0u)
    GPIO_OTYPER(port) = (GPIO_OTYPER(port) & ~config->type_mask) | config->type_value;
  if (config->speed_mask != 0u)
    GPIO_OSPEEDR(port) = (GPIO_OSPEEDR(port) & ~config->speed_mask) | config->speed_value;
  if (config->pull_mask != 0u)
    GPIO_PUPDR(port) = (GPIO_PUPDR(port) & ~config->pull_mask) | config->pull_value;
}

#define DIRECT_MODE_MASK(pin)       (3ul << ((pin) * 2u))
#define DIRECT_MODE_VALUE(pin, val) ((uint32_t)(val) << ((pin) * 2u))
#define DIRECT_PIN_MASK(pin)        GPIO_PIN(pin)
#define DIRECT_PIN_VALUE(pin, val)  ((uint32_t)(val) << (pin))

/**
 * @brief Reads one GPIO input pin.
 * @return Non-zero when the pin is high, otherwise zero.
 */
static inline int direct_get(uint32_t port, uint32_t pin)
{
  return (GPIO_IDR(port) & GPIO_PIN(pin)) != 0u;
}

/** @brief Sets one GPIO output atomically through BSRR. */
static inline void direct_set(uint32_t port, uint32_t pin)
{
  GPIO_BSRR(port) = GPIO_PIN(pin);
}

/** @brief Clears one GPIO output atomically through BRR. */
static inline void direct_clear(uint32_t port, uint32_t pin)
{
  GPIO_BRR(port) = GPIO_PIN(pin);
}


#endif
