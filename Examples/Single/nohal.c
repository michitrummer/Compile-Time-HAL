/**
 * @file nohal.c
 * @brief Single benchmark application for the nohal implementation.
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
  const bsl_Led led = bsl_led4;
  const bsl_Switch sw = bsl_switchB1;
  bsl_boardConfigureGpio();
  while (1) {
    BENCHMARK_LOOP_START();
    if (bsl_switchPressed(sw)) bsl_ledOn(led); else bsl_ledOff(led);
    BENCHMARK_LOOP_END();
  }
}
