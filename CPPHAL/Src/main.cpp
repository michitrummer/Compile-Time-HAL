/**
 * @file main.cpp
 * @brief Standalone application entry point for the CPPHAL implementation.
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
  bsl::Led led0(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw0(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led1(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw1(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led2(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw2(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led3(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw3(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led4(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw4(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led5(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw5(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led6(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw6(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led7(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw7(bsl::Switch::Id::b1, bsl::bindOnly);
  bsl::Led led8(bsl::Led::Id::ld4, bsl::bindOnly);
  bsl::Switch sw8(bsl::Switch::Id::b1, bsl::bindOnly);

  while (1)
  {
    if (sw0.pressed()) led0.on();
    else led0.off();
    if (sw1.pressed()) led1.on();
    else led1.off();
    if (sw2.pressed()) led2.on();
    else led2.off();
    if (sw3.pressed()) led3.on();
    else led3.off();
    if (sw4.pressed()) led4.on();
    else led4.off();
    if (sw5.pressed()) led5.on();
    else led5.off();
    if (sw6.pressed()) led6.on();
    else led6.off();
    if (sw7.pressed()) led7.on();
    else led7.off();
    if (sw8.pressed()) led8.on();
    else led8.off();
  }
}
