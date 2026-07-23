/**
 * @file RCC.cpp
 * @brief Implementation of RCC for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/RCC.h"

#include "./periMemMap.h"

namespace csl {

RCC::Regs& RCC::reg = *reinterpret_cast<Regs*>(
    static_cast<uint32_t>(basePeriphAHB1) + rccBaseOffset);

void RCC::periEnableMask(uint32_t mask) {
  reg.ahb2enr |= mask;
  (void)reg.ahb2enr;
}

void RCC::periEnable(PeriId id) { periEnableMask(static_cast<uint32_t>(id)); }

}  // namespace csl
