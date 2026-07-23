/**
 * @file Pin.cpp
 * @brief Implementation of Pin for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/Pin.h"

namespace csl {

Pin::Pin(Port::Id portId, Id id) : port(portId), bit(bit0 << id) {}

Pin::Pin(Port::Id portId, Id id, Mode mod, Pull pull, Speed speed)
    : Pin(portId, id) {
  Port::enableClocks(Port::clockMask(portId));

  const unsigned int twoBitShift = id << 1u;
  HwReg<uint32_t>::setSubValue<2>(port.modeReg(), mod & (bit0 | bit1),
                                  twoBitShift);

  if ((mod == outPP) || (mod == outOD)) {
    HwReg<uint32_t>::setSubValue<2>(port.speedReg(), speed, twoBitShift);
    if (((mod & static_cast<uint32_t>(gpioOutputTypeMask)) >>
         gpioOutputTypeShift) != 0u)
      HwReg<uint32_t>::setBits(port.typeReg(), bit);
    else
      HwReg<uint32_t>::clearBits(port.typeReg(), bit);
  }

  HwReg<uint32_t>::setSubValue<2>(port.pullReg(), pull, twoBitShift);
}

}  // namespace csl
