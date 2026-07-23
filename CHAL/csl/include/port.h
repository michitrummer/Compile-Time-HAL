/**
 * @file port.h
 * @brief GPIO port register access and aggregated configuration for the C HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_PORT_H_
#define CSL_PORT_H_

#include <stddef.h>
#include <stdint.h>

/** @brief Memory-mapped STM32L433 GPIO register block. */
typedef volatile struct {
  uint32_t moder;   /**< Mode register. */
  uint32_t otyper;  /**< Output type register. */
  uint32_t ospeedr; /**< Output speed register. */
  uint32_t pupdr;   /**< Pull-up/pull-down register. */
  uint32_t idr;     /**< Input data register. */
  uint32_t odr;     /**< Output data register. */
  uint32_t bsrr;    /**< Bit set/reset register. */
  uint32_t lckr;    /**< Configuration lock register. */
  uint32_t afr[2];  /**< Alternate function registers. */
  uint32_t brr;     /**< Bit reset register. */
} csl_Port;

_Static_assert(sizeof(csl_Port) == 0x2Cu,
               "Unexpected GPIO register block size");
_Static_assert(offsetof(csl_Port, moder) == 0x00u,
               "Unexpected GPIO MODER offset");
_Static_assert(offsetof(csl_Port, otyper) == 0x04u,
               "Unexpected GPIO OTYPER offset");
_Static_assert(offsetof(csl_Port, ospeedr) == 0x08u,
               "Unexpected GPIO OSPEEDR offset");
_Static_assert(offsetof(csl_Port, pupdr) == 0x0Cu,
               "Unexpected GPIO PUPDR offset");
_Static_assert(offsetof(csl_Port, idr) == 0x10u, "Unexpected GPIO IDR offset");
_Static_assert(offsetof(csl_Port, odr) == 0x14u, "Unexpected GPIO ODR offset");
_Static_assert(offsetof(csl_Port, bsrr) == 0x18u,
               "Unexpected GPIO BSRR offset");
_Static_assert(offsetof(csl_Port, lckr) == 0x1Cu,
               "Unexpected GPIO LCKR offset");
_Static_assert(offsetof(csl_Port, afr) == 0x20u, "Unexpected GPIO AFR offset");
_Static_assert(offsetof(csl_Port, brr) == 0x28u, "Unexpected GPIO BRR offset");

/** @brief GPIO port identifiers encoded as offsets from the AHB2 peripheral
 * base. */
typedef enum {
  csl_portA = 0x0000ul,
  csl_portB = 0x0400ul,
  csl_portC = 0x0800ul,
  csl_portD = 0x0C00ul,
  csl_portE = 0x1000ul,
  csl_portH = 0x1C00ul
} csl_PortId;

/**
 * @brief Aggregated masks and values for one GPIO port.
 *
 * A zero mask leaves the corresponding register unchanged. This representation
 * allows all pins of one port to be configured with at most one
 * read-modify-write operation per affected register.
 */
typedef struct {
  uint32_t modeMask;
  uint32_t modeValue;
  uint32_t typeMask;
  uint32_t typeValue;
  uint32_t speedMask;
  uint32_t speedValue;
  uint32_t pullMask;
  uint32_t pullValue;
} csl_PortConfiguration;

/**
 * @brief Returns the memory-mapped register block for a GPIO port.
 * @param id GPIO port identifier.
 * @return Pointer to the selected GPIO register block.
 * @note No clock is enabled and no register is modified.
 */
csl_Port* csl_portBind(csl_PortId id);

/**
 * @brief Enables one GPIO port clock and returns its register block.
 * @param id GPIO port identifier.
 * @return Pointer to the selected GPIO register block.
 * @note Kept for the non-aggregated compatibility path.
 */
csl_Port* csl_portInit(csl_PortId id);

/**
 * @brief Enables multiple GPIO port clocks with one RCC register update.
 * @param clockMask OR-combined csl_PeriId values.
 */
void csl_portsEnable(uint32_t clockMask);

/**
 * @brief Applies an aggregated configuration to one GPIO port.
 * @param id GPIO port identifier.
 * @param config Register masks and values to apply.
 */
void csl_portConfigure(csl_PortId id, const csl_PortConfiguration* config);

#endif /* CSL_PORT_H_ */
