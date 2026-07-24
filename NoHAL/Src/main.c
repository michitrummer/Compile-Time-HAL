/**
 * @file main.c
 * @brief Standalone NoHAL application using direct STM32L433 register access.
 *
 * Based on the original NoHAL implementation from Nucleo64HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>

/* Peripheral memory map. */
#define PERIPH_BASE (0x40000000ul)
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000ul)
#define AHB2PERIPH_BASE (PERIPH_BASE + 0x08000000ul)

/* Reset and clock control. */
#define RCC_BASE (AHB1PERIPH_BASE + 0x1000ul)
#define RCC_AHB2ENR(base) (*((volatile uint32_t *)(0x4Cul + (base))))
#define RCC_AHB2ENR_GPIOAEN (0x00000001u)
#define RCC_AHB2ENR_GPIOBEN (0x00000002u)
#define RCC_AHB2ENR_GPIOCEN (0x00000004u)
#define RCC_AHB2ENR_GPIODEN (0x00000008u)
#define RCC_AHB2ENR_GPIOEEN (0x00000010u)
#define RCC_AHB2ENR_GPIOHEN (0x00000080u)

/* AHB2 peripherals. */
#define GPIOA_BASE (AHB2PERIPH_BASE + 0x0000ul)
#define GPIOB_BASE (AHB2PERIPH_BASE + 0x0400ul)
#define GPIOC_BASE (AHB2PERIPH_BASE + 0x0800ul)
#define GPIOD_BASE (AHB2PERIPH_BASE + 0x0C00ul)
#define GPIOE_BASE (AHB2PERIPH_BASE + 0x1000ul)
#define GPIOH_BASE (AHB2PERIPH_BASE + 0x1C00ul)

/* GPIO pins. */
#define GPIO_PIN_0 ((uint16_t)0x0001u)
#define GPIO_PIN_1 ((uint16_t)0x0002u)
#define GPIO_PIN_2 ((uint16_t)0x0004u)
#define GPIO_PIN_3 ((uint16_t)0x0008u)
#define GPIO_PIN_4 ((uint16_t)0x0010u)
#define GPIO_PIN_5 ((uint16_t)0x0020u)
#define GPIO_PIN_6 ((uint16_t)0x0040u)
#define GPIO_PIN_7 ((uint16_t)0x0080u)
#define GPIO_PIN_13 ((uint16_t)0x2000u)

/* GPIO configuration values. */
#define GPIO_MODE_INPUT (0x00000000u)
#define GPIO_MODE_OUTPUT (0x00000001u)
#define GPIO_OUTPUT_PUSH_PULL (0x00000000u)
#define GPIO_OUTPUT_OPEN_DRAIN (0x00000001u)
#define GPIO_NOPULL (0x00000000u)
#define GPIO_PULLUP (0x00000001u)
#define GPIO_PULLDOWN (0x00000002u)
#define GPIO_SPEED_FREQ_LOW (0x00000000u)
#define GPIO_SPEED_FREQ_MEDIUM (0x00000001u)
#define GPIO_SPEED_FREQ_HIGH (0x00000002u)
#define GPIO_SPEED_FREQ_VERY_HIGH (0x00000003u)

/* GPIO registers. */
#define GPIO_MODER(port) (*((volatile uint32_t *)(0x00ul + (port))))
#define GPIO_OTYPER(port) (*((volatile uint32_t *)(0x04ul + (port))))
#define GPIO_OSPEEDR(port) (*((volatile uint32_t *)(0x08ul + (port))))
#define GPIO_PUPDR(port) (*((volatile uint32_t *)(0x0Cul + (port))))
#define GPIO_IDR(port) (*((volatile uint32_t *)(0x10ul + (port))))
#define GPIO_BSRR(port) (*((volatile uint32_t *)(0x18ul + (port))))
#define GPIO_BRR(port) (*((volatile uint32_t *)(0x28ul + (port))))

/* Bit manipulation. */
#define SET_BIT(reg, bit) ((reg) |= ((uint32_t)1u << (uint32_t)(bit)))
#define CLR_BIT(reg, bit) ((reg) &= ~((uint32_t)1u << (uint32_t)(bit)))
#define SET_BITS(reg, bits) ((reg) |= (bits))
#define CLR_BITS(reg, bits) ((reg) &= ~(bits))
#define ARE_BITS_SET(reg, bits) (((reg) & (bits)) == (bits))

/*
 * The real Nucleo LD4/B1 pair used by every Single implementation.
 */
#define SWITCH_0_PORT GPIOC_BASE
#define SWITCH_0_PIN_NR (13u)
#define SWITCH_0_PIN GPIO_PIN_13
#define LED_0_PORT GPIOB_BASE
#define LED_0_PIN_NR (13u)
#define LED_0_PIN GPIO_PIN_13
#define CONFIGURE_INPUT(port, pin, pull)                         \
  do {                                                           \
    CLR_BITS(GPIO_MODER(port), (0x3u << ((pin)*2u)));            \
    SET_BITS(GPIO_MODER(port), (GPIO_MODE_INPUT << ((pin)*2u))); \
    CLR_BITS(GPIO_PUPDR(port), (0x3u << ((pin)*2u)));            \
    SET_BITS(GPIO_PUPDR(port), ((pull) << ((pin)*2u)));          \
  } while (0)

#define CONFIGURE_OUTPUT(port, pin, type, pull, speed)            \
  do {                                                            \
    CLR_BITS(GPIO_MODER(port), (0x3u << ((pin)*2u)));             \
    SET_BITS(GPIO_MODER(port), (GPIO_MODE_OUTPUT << ((pin)*2u))); \
    CLR_BITS(GPIO_OSPEEDR(port), (0x3u << ((pin)*2u)));           \
    SET_BITS(GPIO_OSPEEDR(port), ((speed) << ((pin)*2u)));        \
    CLR_BIT(GPIO_OTYPER(port), pin);                              \
    SET_BITS(GPIO_OTYPER(port), ((type) << (pin)));               \
    CLR_BITS(GPIO_PUPDR(port), (0x3u << ((pin)*2u)));             \
    SET_BITS(GPIO_PUPDR(port), ((pull) << ((pin)*2u)));           \
  } while (0)

int main(void) {
  const uint32_t gpioClocks =
      RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

  if (!ARE_BITS_SET(RCC_AHB2ENR(RCC_BASE), gpioClocks)) {
    SET_BITS(RCC_AHB2ENR(RCC_BASE), gpioClocks);
    volatile uint32_t tmpreg = RCC_AHB2ENR(RCC_BASE);
    (void)tmpreg;
  }

  CONFIGURE_INPUT(SWITCH_0_PORT, SWITCH_0_PIN_NR, GPIO_NOPULL);
  CONFIGURE_OUTPUT(LED_0_PORT, LED_0_PIN_NR, GPIO_OUTPUT_PUSH_PULL, GPIO_NOPULL,
                   GPIO_SPEED_FREQ_LOW);

  while (1) {
    if (ARE_BITS_SET(GPIO_IDR(SWITCH_0_PORT), SWITCH_0_PIN))
      GPIO_BSRR(LED_0_PORT) = LED_0_PIN;
    else
      GPIO_BRR(LED_0_PORT) = LED_0_PIN;

  }
}
