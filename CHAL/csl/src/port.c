/**
 * @file port.c
 * @brief Implementation of port for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/port.h"
#include "../include/rcc.h"
#include "./periMemMap.h"

static uint32_t clockMask(csl_PortId id)
{
  switch (id)
  {
    case csl_portA: return csl_ahb2GpioAEn;
    case csl_portB: return csl_ahb2GpioBEn;
    case csl_portC: return csl_ahb2GpioCEn;
    case csl_portD: return csl_ahb2GpioDEn;
    case csl_portE: return csl_ahb2GpioEEn;
    case csl_portH: return csl_ahb2GpioHEn;
  }
  return 0u;
}

csl_Port* csl_portBind(csl_PortId id)
{
  return (csl_Port*)((uint32_t)basePeriphAHB2 + id);
}

void csl_portsEnable(uint32_t mask)
{
  csl_periEnableMask(mask);
}

void csl_portConfigure(csl_PortId id, const csl_PortConfiguration* config)
{
  csl_Port* const port = csl_portBind(id);

  if (config->modeMask != 0u)
    port->moder = (port->moder & ~config->modeMask) | config->modeValue;
  if (config->typeMask != 0u)
    port->otyper = (port->otyper & ~config->typeMask) | config->typeValue;
  if (config->speedMask != 0u)
    port->ospeedr = (port->ospeedr & ~config->speedMask) | config->speedValue;
  if (config->pullMask != 0u)
    port->pupdr = (port->pupdr & ~config->pullMask) | config->pullValue;
}

csl_Port* csl_portInit(csl_PortId id)
{
  csl_portsEnable(clockMask(id));
  return csl_portBind(id);
}
