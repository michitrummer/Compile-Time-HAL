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

#include <csl/include/GpioConfig.h>
#include "./Led.h"
#include "./Switch.h"

namespace bsl
{

/** @brief Applies aggregated GPIO initialization for the compile-time BSL mappings. */
struct BoardConfig
{
  /** @brief Configures the standard LD4/B1 board mapping. */
  static void apply()
  {
    typedef LedTraits<LedId::ld4> Led;
    typedef SwitchTraits<SwitchId::b1> Switch;
    csl::GpioClockConfiguration<Led::clockMask | Switch::clockMask>::apply();
    csl::PortConfiguration<Led::portId, Led>::apply();
    csl::PortConfiguration<Switch::portId, Switch>::apply();
  }

  /** @brief Configures all GPIOs used by the diversified MultiEx benchmark. */
  static void applyMultiEx()
  {
    typedef LedTraits<LedId::ld4> L0; typedef SwitchTraits<SwitchId::b1> S0;
    typedef LedTraits<LedId::ex1> L1; typedef SwitchTraits<SwitchId::ex1> S1;
    typedef LedTraits<LedId::ex2> L2; typedef SwitchTraits<SwitchId::ex2> S2;
    typedef LedTraits<LedId::ex3> L3; typedef SwitchTraits<SwitchId::ex3> S3;
    typedef LedTraits<LedId::ex4> L4; typedef SwitchTraits<SwitchId::ex4> S4;
    typedef LedTraits<LedId::ex5> L5; typedef SwitchTraits<SwitchId::ex5> S5;
    typedef LedTraits<LedId::ex6> L6; typedef SwitchTraits<SwitchId::ex6> S6;
    typedef LedTraits<LedId::ex7> L7; typedef SwitchTraits<SwitchId::ex7> S7;
    typedef LedTraits<LedId::ex8> L8; typedef SwitchTraits<SwitchId::ex8> S8;

    csl::GpioClockConfiguration<
      L0::clockMask | S0::clockMask | L1::clockMask | S1::clockMask |
      L2::clockMask | S2::clockMask | L3::clockMask | S3::clockMask |
      L4::clockMask | S4::clockMask | L5::clockMask | S5::clockMask |
      L6::clockMask | S6::clockMask | L7::clockMask | S7::clockMask |
      L8::clockMask | S8::clockMask>::apply();

    csl::PortConfiguration<csl::port::a,
      S1, S2,
      S3, S4>::apply();
    csl::PortConfiguration<csl::port::b,
      L1, L2,
      L3, L4,
      L5, L6,
      L0>::apply();
    csl::PortConfiguration<csl::port::c,
      S5, S6,
      S0>::apply();
    csl::PortConfiguration<csl::port::d, S7>::apply();
    csl::PortConfiguration<csl::port::e,
      L7, S8>::apply();
    csl::PortConfiguration<csl::port::h, L8>::apply();
  }
};

} // namespace bsl
#endif
