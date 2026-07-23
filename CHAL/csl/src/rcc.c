/**
 * @file rcc.c
 * @brief Implementation of rcc for the hardware abstraction layer.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#include "../include/rcc.h"

#include <stdbool.h>
#include <stddef.h>

#include "../include/hwReg.h"

#include "./periMemMap.h"

/**
  * @brief Reset and clock control.
  */
typedef volatile struct
{
	uint32_t cr;          ///< RCC clock control register
	uint32_t icscr;       ///< RCC internal clock sources calibration register
	uint32_t cfgr;        ///< RCC clock configuration register
	uint32_t pllcfgr;     ///< RCC system PLL configuration register
	uint32_t pllsai1cfgr; ///< RCC PLL SAI1 configuration register
	uint32_t reserved0;   ///< Reserved
	uint32_t cier;        ///< RCC clock interrupt enable register
	uint32_t cifr;        ///< RCC clock interrupt flag register
	uint32_t cicr;        ///< RCC clock interrupt clear register
	uint32_t reserved1;   ///< Reserved
	uint32_t ahb1rstr;    ///< RCC AHB1 peripheral reset register
	uint32_t ahb2rstr;    ///< RCC AHB2 peripheral reset register
	uint32_t ahb3rstr;    ///< RCC AHB3 peripheral reset register
	uint32_t reserved2;   ///< Reserved
	uint32_t apb1rstr1;   ///< RCC APB1 peripheral reset register 1
	uint32_t apb1rstr2;   ///< RCC APB1 peripheral reset register 2
	uint32_t apb2rstr;    ///< RCC APB2 peripheral reset register
	uint32_t reserved3;   ///< Reserved
	uint32_t ahb1enr;     ///< RCC AHB1 peripheral clocks enable register
	uint32_t ahb2enr;     ///< RCC AHB2 peripheral clocks enable register
	uint32_t ahb3enr;     ///< RCC AHB3 peripheral clocks enable register
	uint32_t reserved4;   ///< Reserved
	uint32_t apb1enr1;    ///< RCC APB1 peripheral clocks enable register 1
	uint32_t apb1enr2;    ///< RCC APB1 peripheral clocks enable register 2
	uint32_t apb2enr;     ///< RCC APB2 peripheral clocks enable register
	uint32_t reserved5;   ///< Reserved
	uint32_t ahb1smenr;   ///< RCC AHB1 peripheral clocks enable in sleep and stop modes register
	uint32_t ahb2smenr;   ///< RCC AHB2 peripheral clocks enable in sleep and stop modes register
	uint32_t ahb3smenr;   ///< RCC AHB3 peripheral clocks enable in sleep and stop modes register
	uint32_t reserved6;   ///< Reserved
	uint32_t apb1smenr1;  ///< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 1
	uint32_t apb1smenr2;  ///< RCC APB1 peripheral clocks enable in sleep mode and stop modes register 2
	uint32_t apb2smenr;   ///< RCC APB2 peripheral clocks enable in sleep mode and stop modes register
	uint32_t reserved7;   ///< Reserved
	uint32_t ccipr;       ///< RCC peripherals independent clock configuration register
	uint32_t reserved8;   ///< Reserved
	uint32_t bdcr;        ///< RCC backup domain control register
	uint32_t csr;         ///< RCC clock control & status register
	uint32_t crrcr;       ///< RCC clock recovery RC register
} RCC;

_Static_assert(sizeof(RCC) == 0x9Cu, "Unexpected RCC register block size");
_Static_assert(offsetof(RCC, cr) == 0x00u, "Unexpected RCC CR offset");
_Static_assert(offsetof(RCC, ahb2enr) == 0x4Cu, "Unexpected RCC AHB2ENR offset");
_Static_assert(offsetof(RCC, crrcr) == 0x98u, "Unexpected RCC CRRCR offset");

enum
{
	rccBaseOffset = 0x1000ul ///< RCC base offset
};

static RCC* const rcc = (RCC*)( (uint32_t)basePeriphAHB1 + rccBaseOffset ); ///< RCC instance

void csl_rccInit()
{
	static bool initDone = false;
	if( !initDone )
	{
		initDone = true;
		// RCC init goes here
	}
}

void csl_periEnableMask(uint32_t mask)
{
	rcc->ahb2enr |= mask;
	(void)rcc->ahb2enr;
}

void csl_periEnable(csl_PeriId id)
{
	csl_periEnableMask((uint32_t)id);
}
