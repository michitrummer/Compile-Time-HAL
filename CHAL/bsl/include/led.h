/**
 * @file led.h
 * @brief Board-support LED abstraction for the C HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_LED_H_
#define BSL_LED_H_

#include <csl/include/pin.h>

/** @brief Logical LED identifiers used by the board and benchmark scenarios. */
typedef enum
{
  bsl_led4,
  bsl_ledEx1, bsl_ledEx2, bsl_ledEx3, bsl_ledEx4,
  bsl_ledEx5, bsl_ledEx6, bsl_ledEx7, bsl_ledEx8
} bsl_LedId;

/** @brief LED object containing the bound GPIO pin handle. */
typedef struct
{
  csl_Pin pin;
} bsl_Led;

/**
 * @brief Binds a logical LED to its board-specific GPIO without configuring it.
 * @param led Destination LED object.
 * @param id Logical LED identifier.
 */
static inline void bsl_ledBind(bsl_Led* led, bsl_LedId id)
{
  switch (id)
  {
    case bsl_led4:   csl_pinBind(&led->pin, csl_portB, csl_pin13); break;
    case bsl_ledEx1: csl_pinBind(&led->pin, csl_portB, csl_pin0);  break;
    case bsl_ledEx2: csl_pinBind(&led->pin, csl_portB, csl_pin1);  break;
    case bsl_ledEx3: csl_pinBind(&led->pin, csl_portB, csl_pin2);  break;
    case bsl_ledEx4: csl_pinBind(&led->pin, csl_portB, csl_pin3);  break;
    case bsl_ledEx5: csl_pinBind(&led->pin, csl_portB, csl_pin4);  break;
    case bsl_ledEx6: csl_pinBind(&led->pin, csl_portB, csl_pin5);  break;
    case bsl_ledEx7: csl_pinBind(&led->pin, csl_portE, csl_pin6);  break;
    case bsl_ledEx8: csl_pinBind(&led->pin, csl_portH, csl_pin0);  break;
  }
}

/** @brief Drives the LED output low. */
static inline void bsl_ledOff(bsl_Led* led)
{
  csl_pinClear(&led->pin);
}

/** @brief Drives the LED output high. */
static inline void bsl_ledOn(bsl_Led* led)
{
  csl_pinSet(&led->pin);
}

#endif /* BSL_LED_H_ */
