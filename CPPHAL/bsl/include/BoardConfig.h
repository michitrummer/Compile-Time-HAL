/**
 * @file BoardConfig.h
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
#include <csl/include/Port.h>
#include <csl/include/RCC.h>
namespace bsl {
/** @brief Applies the GPIO configuration required by the benchmark board
 * mappings. */
struct BoardConfig {
  /** @brief Configures LD4 and B1 using aggregated per-port register updates.
   */
  static void apply() {
    static const csl::Port::Configuration ledPort = {
        3u << 26u, 1u << 26u, 1u << 13u, 0u, 3u << 26u, 0u, 3u << 26u, 0u};
    static const csl::Port::Configuration switchPort = {
        3u << 26u, 0u, 0u, 0u, 0u, 0u, 3u << 26u, 0u};
    csl::Port::enableClocks(csl::RCC::ahb2GpioBEn | csl::RCC::ahb2GpioCEn);
    csl::Port::configure(csl::Port::b, ledPort);
    csl::Port::configure(csl::Port::c, switchPort);
  }
  /** @brief Configures all GPIOs used by the diversified MultiEx benchmark. */
  static void applyMultiEx() {
    static const csl::Port::Configuration a = {
        0x000000FFu, 0x00000000u, 0u, 0u, 0u, 0u, 0x000000FFu, 0x00000049u};
    static const csl::Port::Configuration b = {
        0x0C000FFFu, 0x04000555u, 0x0000203Fu, 0x0000002Au,
        0x0C000FFFu, 0x00000939u, 0x0C000FFFu, 0x00000249u};
    static const csl::Port::Configuration c = {
        0x0C000F00u, 0x00000000u, 0u, 0u, 0u, 0u, 0x0C000F00u, 0x00000200u};
    static const csl::Port::Configuration d = {
        0x00003000u, 0x00000000u, 0u, 0u, 0u, 0u, 0x00003000u, 0x00001000u};
    static const csl::Port::Configuration e = {
        0x0000F000u, 0x00001000u, 0x00000040u, 0u,
        0x00003000u, 0x00003000u, 0x0000F000u, 0x00009000u};
    static const csl::Port::Configuration h = {
        0x00000003u, 0x00000001u, 0x00000001u, 0x00000001u,
        0x00000003u, 0u,          0x00000003u, 0x00000002u};
    csl::Port::enableClocks(csl::RCC::ahb2GpioAEn | csl::RCC::ahb2GpioBEn |
                            csl::RCC::ahb2GpioCEn | csl::RCC::ahb2GpioDEn |
                            csl::RCC::ahb2GpioEEn | csl::RCC::ahb2GpioHEn);
    csl::Port::configure(csl::Port::a, a);
    csl::Port::configure(csl::Port::b, b);
    csl::Port::configure(csl::Port::c, c);
    csl::Port::configure(csl::Port::d, d);
    csl::Port::configure(csl::Port::e, e);
    csl::Port::configure(csl::Port::h, h);
  }
};
}  // namespace bsl
#endif
