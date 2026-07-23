/**
 * @file pin.h
 * @brief GPIO pin abstraction for the C HAL.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CSL_PIN_H_
#define CSL_PIN_H_

#include <stdbool.h>
#include <stdint.h>

#include "./hwReg.h"
#include "./port.h"

/** @brief STM32L433 GPIO pin numbers. */
typedef enum {
  csl_pin0 = 0,
  csl_pin1,
  csl_pin2,
  csl_pin3,
  csl_pin4,
  csl_pin5,
  csl_pin6,
  csl_pin7,
  csl_pin8,
  csl_pin9,
  csl_pin10,
  csl_pin11,
  csl_pin12,
  csl_pin13,
  csl_pin14,
  csl_pin15
} csl_PinId;

/** @brief Runtime GPIO pin handle containing a bound port and pin mask. */
typedef struct {
  csl_Port* port; /**< Bound GPIO register block. */
  uint32_t bit;   /**< Single-bit mask for the selected pin. */
} csl_Pin;

/** @brief GPIO operating modes used by the benchmark HAL. */
typedef enum {
  csl_pinInp = 0x00000000u,   /**< Digital input. */
  csl_pinOutPP = 0x00000001u, /**< Push-pull output. */
  csl_pinOutOD = 0x00000011u  /**< Open-drain output. */
} csl_PinMode;

/** @brief GPIO pull resistor configuration. */
typedef enum {
  csl_pinNoPull = 0x00000000u,  /**< No pull resistor. */
  csl_pinPullUp = 0x00000001u,  /**< Enable pull-up. */
  csl_pinPullDown = 0x00000002u /**< Enable pull-down. */
} csl_PinPull;

/** @brief GPIO output speed encoding for STM32L433 OSPEEDR. */
typedef enum {
  csl_pinFreqLow = 0x00000000u,     /**< Low speed. */
  csl_pinFreqMid = 0x00000001u,     /**< Medium speed. */
  csl_pinFreqHigh = 0x00000002u,    /**< High speed. */
  csl_pinFreqVeryHigh = 0x00000003u /**< Very high speed. */
} csl_PinSpeed;

/**
 * @brief Binds a pin handle without modifying GPIO configuration registers.
 * @param pin Destination handle.
 * @param port GPIO port identifier.
 * @param id GPIO pin number.
 * @pre @p pin must point to writable storage.
 */
void csl_pinBind(csl_Pin* pin, csl_PortId port, csl_PinId id);

/**
 * @brief Enables the port clock, binds the handle, and configures one GPIO pin.
 * @param pin Destination handle.
 * @param port GPIO port identifier.
 * @param id GPIO pin number.
 * @param mode Input, push-pull output, or open-drain output mode.
 * @param pull Pull resistor configuration.
 * @param speed Output speed. Ignored for input mode.
 * @pre @p pin must point to writable storage.
 * @note The aggregated benchmark path uses csl_pinBind() and
 * csl_portConfigure() instead, so several pins can share one update per GPIO
 * register.
 */
void csl_pinInit(csl_Pin* pin, csl_PortId port, csl_PinId id, csl_PinMode mode,
                 csl_PinPull pull, csl_PinSpeed speed);

/**
 * @brief Sets the bound GPIO output atomically through BSRR.
 * @param pin Bound pin handle.
 */
static inline void csl_pinSet(csl_Pin* pin) { pin->port->bsrr = pin->bit; }

/**
 * @brief Clears the bound GPIO output atomically through BRR.
 * @param pin Bound pin handle.
 */
static inline void csl_pinClear(csl_Pin* pin) { pin->port->brr = pin->bit; }

/**
 * @brief Toggles the bound GPIO output based on the current ODR state.
 * @param pin Bound pin handle.
 */
static inline void csl_pinToggle(csl_Pin* pin) {
  if (csl_hwReg32AreBitsSet(&(pin->port->odr), pin->bit))
    csl_pinClear(pin);
  else
    csl_pinSet(pin);
}

/**
 * @brief Reads the bound GPIO input state.
 * @param pin Bound pin handle.
 * @return true when the input is high; false otherwise.
 */
static inline bool csl_pinGet(const csl_Pin* pin) {
  return csl_hwReg32AreBitsSet(&(pin->port->idr), pin->bit);
}

#endif /* CSL_PIN_H_ */
