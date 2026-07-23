/**
 * @file main.cpp
 * @brief Standalone application entry point for the CPPCTHAL implementation.
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

int main(void)
{
  bsl::BoardConfig::apply();
  bsl::Led<bsl::LedId::ld4> led0;
  bsl::Switch<bsl::SwitchId::b1> sw0;


  while (1)
  {
    if (sw0.pressed()) led0.on();
    else led0.off();
  }
}
