/**
 * @file chal.c
 * @brief MultiCall benchmark application for the chal implementation.
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

int main(void){ bsl_boardConfigureGpio();
  bsl_Led led0; bsl_Switch sw0; bsl_ledBind(&led0,bsl_led4); bsl_switchBind(&sw0,bsl_switchB1);
  bsl_Led led1; bsl_Switch sw1; bsl_ledBind(&led1,bsl_led4); bsl_switchBind(&sw1,bsl_switchB1);
  bsl_Led led2; bsl_Switch sw2; bsl_ledBind(&led2,bsl_led4); bsl_switchBind(&sw2,bsl_switchB1);
  bsl_Led led3; bsl_Switch sw3; bsl_ledBind(&led3,bsl_led4); bsl_switchBind(&sw3,bsl_switchB1);
  bsl_Led led4; bsl_Switch sw4; bsl_ledBind(&led4,bsl_led4); bsl_switchBind(&sw4,bsl_switchB1);
  bsl_Led led5; bsl_Switch sw5; bsl_ledBind(&led5,bsl_led4); bsl_switchBind(&sw5,bsl_switchB1);
  bsl_Led led6; bsl_Switch sw6; bsl_ledBind(&led6,bsl_led4); bsl_switchBind(&sw6,bsl_switchB1);
  bsl_Led led7; bsl_Switch sw7; bsl_ledBind(&led7,bsl_led4); bsl_switchBind(&sw7,bsl_switchB1);
  bsl_Led led8; bsl_Switch sw8; bsl_ledBind(&led8,bsl_led4); bsl_switchBind(&sw8,bsl_switchB1);
 while(1){ BENCHMARK_LOOP_START();
    if(bsl_switchPressed(&sw0)) bsl_ledOn(&led0); else bsl_ledOff(&led0);
    if(bsl_switchPressed(&sw1)) bsl_ledOn(&led1); else bsl_ledOff(&led1);
    if(bsl_switchPressed(&sw2)) bsl_ledOn(&led2); else bsl_ledOff(&led2);
    if(bsl_switchPressed(&sw3)) bsl_ledOn(&led3); else bsl_ledOff(&led3);
    if(bsl_switchPressed(&sw4)) bsl_ledOn(&led4); else bsl_ledOff(&led4);
    if(bsl_switchPressed(&sw5)) bsl_ledOn(&led5); else bsl_ledOff(&led5);
    if(bsl_switchPressed(&sw6)) bsl_ledOn(&led6); else bsl_ledOff(&led6);
    if(bsl_switchPressed(&sw7)) bsl_ledOn(&led7); else bsl_ledOff(&led7);
    if(bsl_switchPressed(&sw8)) bsl_ledOn(&led8); else bsl_ledOff(&led8);
 BENCHMARK_LOOP_END(); } }
