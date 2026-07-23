/**
 * @file pin.c
 * @brief Implementation of pin for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/pin.h"

#include "../include/bits.h"

enum
{
	gpioOutputTypeMask = csl_bit4,
	gpioOutputTypeShift = 4u
};

void csl_pinBind(csl_Pin* pin, csl_PortId port, csl_PinId id)
{
	pin->port = csl_portBind(port);
	pin->bit = csl_bit0 << id;
}

void csl_pinInit(csl_Pin* pin,
				 csl_PortId port,
				 csl_PinId id,
				 csl_PinMode mod,
				 csl_PinPull pull,
				 csl_PinSpeed speed)
{
	csl_portInit(port);
	csl_pinBind(pin, port, id);

	// set mode
	const unsigned int twoBitShift = (id << 1u);
	csl_hwReg32Set2BitSubValue(&(pin->port->moder), (mod & (csl_bit0|csl_bit1)), twoBitShift);

	if( (mod == csl_pinOutPP) || (mod == csl_pinOutOD) )
	{
		// set speed
		csl_hwReg32Set2BitSubValue(&(pin->port->ospeedr), speed, twoBitShift);

		// set open drain if selected
		if( ((mod & (uint32_t)gpioOutputTypeMask) >> gpioOutputTypeShift) )
			csl_hwReg32SetBits(&(pin->port->otyper), pin->bit);
		else
			csl_hwReg32ClearBits(&(pin->port->otyper), pin->bit);
	}

	// set pull
	csl_hwReg32Set2BitSubValue(&(pin->port->pupdr), pull, twoBitShift);
}
