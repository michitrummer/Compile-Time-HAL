/**
 * @file boardConfig.h
 * @brief Board-level GPIO initialization used by the benchmark applications.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_BOARD_CONFIG_H_
#define BSL_BOARD_CONFIG_H_
#include <csl/include/port.h>
#include <csl/include/rcc.h>

/** @brief Configures the LD4 and B1 GPIOs using aggregated per-port updates. */
static inline void bsl_boardConfigureGpio(void) {
  static const csl_PortConfiguration ledPort = {
      3u << 26u, 1u << 26u, 1u << 13u, 0u, 3u << 26u, 0u, 3u << 26u, 0u};
  static const csl_PortConfiguration switchPort = {
      3u << 26u, 0u, 0u, 0u, 0u, 0u, 3u << 26u, 0u};
  csl_portsEnable(csl_ahb2GpioBEn | csl_ahb2GpioCEn);
  csl_portConfigure(csl_portB, &ledPort);
  csl_portConfigure(csl_portC, &switchPort);
}

/** @brief Configures all GPIOs used by the diversified MultiEx benchmark. */
static inline void bsl_boardConfigureGpioMultiEx(void) {
  static const csl_PortConfiguration portA = {
      0x000000FFu, 0x00000000u, 0u, 0u, 0u, 0u, 0x000000FFu, 0x00000049u};
  static const csl_PortConfiguration portB = {
      0x0C000FFFu, 0x04000555u, 0x0000203Fu, 0x0000002Au,
      0x0C000FFFu, 0x00000939u, 0x0C000FFFu, 0x00000249u};
  static const csl_PortConfiguration portC = {
      0x0C000F00u, 0x00000000u, 0u, 0u, 0u, 0u, 0x0C000F00u, 0x00000200u};
  static const csl_PortConfiguration portD = {
      0x00003000u, 0x00000000u, 0u, 0u, 0u, 0u, 0x00003000u, 0x00001000u};
  static const csl_PortConfiguration portE = {
      0x0000F000u, 0x00001000u, 0x00000040u, 0u,
      0x00003000u, 0x00003000u, 0x0000F000u, 0x00009000u};
  static const csl_PortConfiguration portH = {
      0x00000003u, 0x00000001u, 0x00000001u, 0x00000001u,
      0x00000003u, 0u,          0x00000003u, 0x00000002u};
  csl_portsEnable(csl_ahb2GpioAEn | csl_ahb2GpioBEn | csl_ahb2GpioCEn |
                  csl_ahb2GpioDEn | csl_ahb2GpioEEn | csl_ahb2GpioHEn);
  csl_portConfigure(csl_portA, &portA);
  csl_portConfigure(csl_portB, &portB);
  csl_portConfigure(csl_portC, &portC);
  csl_portConfigure(csl_portD, &portD);
  csl_portConfigure(csl_portE, &portE);
  csl_portConfigure(csl_portH, &portH);
}
#endif
