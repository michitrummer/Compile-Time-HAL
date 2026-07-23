/**
 * @file Port.cpp
 * @brief Implementation of Port for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/Port.h"
#include "../include/RCC.h"
#include "./periMemMap.h"

namespace csl
{

Port::Port(Id id) :
  reg(*reinterpret_cast<Regs*>(static_cast<uint32_t>(basePeriphAHB2) + id))
{
}

uint32_t Port::clockMask(Id id)
{
  switch (id)
  {
    case a: return RCC::ahb2GpioAEn;
    case b: return RCC::ahb2GpioBEn;
    case c: return RCC::ahb2GpioCEn;
    case d: return RCC::ahb2GpioDEn;
    case e: return RCC::ahb2GpioEEn;
    case h: return RCC::ahb2GpioHEn;
  }
  return 0u;
}

void Port::enableClocks(uint32_t mask)
{
  RCC::periEnableMask(mask);
}

void Port::configure(Id id, const Configuration& config)
{
  Port port(id);
  if (config.modeMask != 0u)
    port.modeReg() = (port.modeReg() & ~config.modeMask) | config.modeValue;
  if (config.typeMask != 0u)
    port.typeReg() = (port.typeReg() & ~config.typeMask) | config.typeValue;
  if (config.speedMask != 0u)
    port.speedReg() = (port.speedReg() & ~config.speedMask) | config.speedValue;
  if (config.pullMask != 0u)
    port.pullReg() = (port.pullReg() & ~config.pullMask) | config.pullValue;
}

} // namespace csl
