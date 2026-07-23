/**
 * @file cppcthal.cpp
 * @brief Single benchmark application for the cppcthal implementation.
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
#define BENCHMARK_LOOP_START() __asm volatile(".global benchmark_loop_start\nbenchmark_loop_start:" ::: "memory")
#define BENCHMARK_LOOP_END() __asm volatile(".global benchmark_loop_end\nbenchmark_loop_end:" ::: "memory")

int main(void){ bsl::BoardConfig::apply(); bsl::Led<bsl::LedId::ld4> led; bsl::Switch<bsl::SwitchId::b1> sw; while(1){ BENCHMARK_LOOP_START(); if(sw.pressed()) led.on(); else led.off(); BENCHMARK_LOOP_END(); } }
