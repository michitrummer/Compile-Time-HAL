/**
 * @file main.c
 * @brief Standalone application entry point for the CHAL implementation.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bsl/include/boardConfig.h>
#include <bsl/include/led.h>
#include <bsl/include/switch.h>

int main(void) {
  bsl_boardConfigureGpio();
  bsl_Led led0;
  bsl_Switch sw0;
  bsl_ledBind(&led0, bsl_led4);
  bsl_switchBind(&sw0, bsl_switchB1);

  while (1) {
    if (bsl_switchPressed(&sw0))
      bsl_ledOn(&led0);
    else
      bsl_ledOff(&led0);
  }
}
