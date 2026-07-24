/**
 * @file cubemx-hal.c
 * @brief MultiExCall benchmark application for the cubemx-hal implementation.
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
#define BENCHMARK_LOOP_START() __asm volatile(".global benchmark_loop_start\nbenchmark_loop_start:" ::: "memory")
#define BENCHMARK_LOOP_END() __asm volatile(".global benchmark_loop_end\nbenchmark_loop_end:" ::: "memory")

int main(void)
{
  const bsl_Led led0=bsl_led4; const bsl_Switch sw0=bsl_switchB1;
  const bsl_Led led1=bsl_ledEx1; const bsl_Switch sw1=bsl_switchEx1;
  const bsl_Led led2=bsl_ledEx2; const bsl_Switch sw2=bsl_switchEx2;
  const bsl_Led led3=bsl_ledEx3; const bsl_Switch sw3=bsl_switchEx3;
  const bsl_Led led4=bsl_ledEx4; const bsl_Switch sw4=bsl_switchEx4;
  const bsl_Led led5=bsl_ledEx5; const bsl_Switch sw5=bsl_switchEx5;
  const bsl_Led led6=bsl_ledEx6; const bsl_Switch sw6=bsl_switchEx6;
  const bsl_Led led7=bsl_ledEx7; const bsl_Switch sw7=bsl_switchEx7;
  const bsl_Led led8=bsl_ledEx8; const bsl_Switch sw8=bsl_switchEx8;
  bsl_boardConfigureGpioMultiEx();
  while (1) {
    BENCHMARK_LOOP_START();
    if (bsl_switchPressed(sw0)) bsl_ledOn(led0); else bsl_ledOff(led0);
    if (bsl_switchPressed(sw1)) bsl_ledOn(led1); else bsl_ledOff(led1);
    if (bsl_switchPressed(sw2)) bsl_ledOn(led2); else bsl_ledOff(led2);
    if (bsl_switchPressed(sw3)) bsl_ledOn(led3); else bsl_ledOff(led3);
    if (bsl_switchPressed(sw4)) bsl_ledOn(led4); else bsl_ledOff(led4);
    if (bsl_switchPressed(sw5)) bsl_ledOn(led5); else bsl_ledOff(led5);
    if (bsl_switchPressed(sw6)) bsl_ledOn(led6); else bsl_ledOff(led6);
    if (bsl_switchPressed(sw7)) bsl_ledOn(led7); else bsl_ledOff(led7);
    if (bsl_switchPressed(sw8)) bsl_ledOn(led8); else bsl_ledOff(led8);
    BENCHMARK_LOOP_END();
  }
}
