/**
 * @file switch.h
 * @brief Board-support switch abstraction for the C HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_SWITCH_H_
#define BSL_SWITCH_H_

#include <csl/include/pin.h>

/** @brief Logical switch identifiers used by the board and benchmark scenarios.
 */
typedef enum {
  bsl_switchB1,
  bsl_switchEx1,
  bsl_switchEx2,
  bsl_switchEx3,
  bsl_switchEx4,
  bsl_switchEx5,
  bsl_switchEx6,
  bsl_switchEx7,
  bsl_switchEx8
} bsl_SwitchId;

/** @brief Switch object containing the bound GPIO pin handle. */
typedef struct {
  csl_Pin pin;
} bsl_Switch;

/**
 * @brief Binds a logical switch to its board-specific GPIO without configuring
 * it.
 * @param sw Destination switch object.
 * @param id Logical switch identifier.
 */
static inline void bsl_switchBind(bsl_Switch* sw, bsl_SwitchId id) {
  switch (id) {
    case bsl_switchB1:
      csl_pinBind(&sw->pin, csl_portC, csl_pin13);
      break;
    case bsl_switchEx1:
      csl_pinBind(&sw->pin, csl_portA, csl_pin0);
      break;
    case bsl_switchEx2:
      csl_pinBind(&sw->pin, csl_portA, csl_pin1);
      break;
    case bsl_switchEx3:
      csl_pinBind(&sw->pin, csl_portA, csl_pin2);
      break;
    case bsl_switchEx4:
      csl_pinBind(&sw->pin, csl_portA, csl_pin3);
      break;
    case bsl_switchEx5:
      csl_pinBind(&sw->pin, csl_portC, csl_pin4);
      break;
    case bsl_switchEx6:
      csl_pinBind(&sw->pin, csl_portC, csl_pin5);
      break;
    case bsl_switchEx7:
      csl_pinBind(&sw->pin, csl_portD, csl_pin6);
      break;
    case bsl_switchEx8:
      csl_pinBind(&sw->pin, csl_portE, csl_pin7);
      break;
  }
}

/**
 * @brief Reads the logical switch input.
 * @param sw Bound switch object.
 * @return true when the GPIO input is high; false otherwise.
 */
static inline bool bsl_switchPressed(const bsl_Switch* sw) {
  return csl_pinGet(&sw->pin);
}

#endif /* BSL_SWITCH_H_ */
