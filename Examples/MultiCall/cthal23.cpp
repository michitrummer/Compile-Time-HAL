/**
 * @file cthal23.cpp
 * @brief MultiCall benchmark application for the cthal23 implementation.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include <bsl/include/BoardConfig.h>
#include <bsl/include/Led.h>
#include <bsl/include/Switch.h>
#define BENCHMARK_LOOP_START()                                            \
  __asm volatile(".global benchmark_loop_start\nbenchmark_loop_start:" :: \
                     : "memory")
#define BENCHMARK_LOOP_END()                                                   \
  __asm volatile(".global benchmark_loop_end\nbenchmark_loop_end:" ::: "memor" \
                                                                       "y")

int main(void) {
  bsl::BoardConfig::apply();
  bsl::Led<bsl::LedId::ld4> led0;
  bsl::Switch<bsl::SwitchId::b1> sw0;
  bsl::Led<bsl::LedId::ld4> led1;
  bsl::Switch<bsl::SwitchId::b1> sw1;
  bsl::Led<bsl::LedId::ld4> led2;
  bsl::Switch<bsl::SwitchId::b1> sw2;
  bsl::Led<bsl::LedId::ld4> led3;
  bsl::Switch<bsl::SwitchId::b1> sw3;
  bsl::Led<bsl::LedId::ld4> led4;
  bsl::Switch<bsl::SwitchId::b1> sw4;
  bsl::Led<bsl::LedId::ld4> led5;
  bsl::Switch<bsl::SwitchId::b1> sw5;
  bsl::Led<bsl::LedId::ld4> led6;
  bsl::Switch<bsl::SwitchId::b1> sw6;
  bsl::Led<bsl::LedId::ld4> led7;
  bsl::Switch<bsl::SwitchId::b1> sw7;
  bsl::Led<bsl::LedId::ld4> led8;
  bsl::Switch<bsl::SwitchId::b1> sw8;
  while (1) {
    BENCHMARK_LOOP_START();
    if (sw0.pressed())
      led0.on();
    else
      led0.off();
    if (sw1.pressed())
      led1.on();
    else
      led1.off();
    if (sw2.pressed())
      led2.on();
    else
      led2.off();
    if (sw3.pressed())
      led3.on();
    else
      led3.off();
    if (sw4.pressed())
      led4.on();
    else
      led4.off();
    if (sw5.pressed())
      led5.on();
    else
      led5.off();
    if (sw6.pressed())
      led6.on();
    else
      led6.off();
    if (sw7.pressed())
      led7.on();
    else
      led7.off();
    if (sw8.pressed())
      led8.on();
    else
      led8.off();
    BENCHMARK_LOOP_END();
  }
}
