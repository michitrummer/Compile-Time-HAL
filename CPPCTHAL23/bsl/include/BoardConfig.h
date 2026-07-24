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

#ifndef BSL23_BOARD_CONFIG_H_
#define BSL23_BOARD_CONFIG_H_

#include <csl/include/GpioConfig.h>

#include "./Led.h"
#include "./Switch.h"

namespace bsl {

/** @brief Applies aggregated GPIO initialization for all C++23 BSL mappings. */
struct BoardConfig {
  /** @brief Configures the standard LD4/B1 board mapping. */
  static void apply() {
    using L = LedTraits<LedId::ld4>;
    using S = SwitchTraits<SwitchId::b1>;
    csl::GpioClockConfiguration<L::clockMask | S::clockMask>::apply();
    csl::PortConfiguration<L::portId, L::configuration>::apply();
    csl::PortConfiguration<S::portId, S::configuration>::apply();
  }

  /** @brief Configures all GPIOs used by the diversified MultiEx benchmark. */
  static void applyMultiEx() {
    using L0 = LedTraits<LedId::ld4>;
    using S0 = SwitchTraits<SwitchId::b1>;
    using L1 = LedTraits<LedId::ex1>;
    using S1 = SwitchTraits<SwitchId::ex1>;
    using L2 = LedTraits<LedId::ex2>;
    using S2 = SwitchTraits<SwitchId::ex2>;
    using L3 = LedTraits<LedId::ex3>;
    using S3 = SwitchTraits<SwitchId::ex3>;
    using L4 = LedTraits<LedId::ex4>;
    using S4 = SwitchTraits<SwitchId::ex4>;
    using L5 = LedTraits<LedId::ex5>;
    using S5 = SwitchTraits<SwitchId::ex5>;
    using L6 = LedTraits<LedId::ex6>;
    using S6 = SwitchTraits<SwitchId::ex6>;
    using L7 = LedTraits<LedId::ex7>;
    using S7 = SwitchTraits<SwitchId::ex7>;
    using L8 = LedTraits<LedId::ex8>;
    using S8 = SwitchTraits<SwitchId::ex8>;

    csl::GpioClockConfiguration<
        L0::clockMask | S0::clockMask | L1::clockMask | S1::clockMask |
        L2::clockMask | S2::clockMask | L3::clockMask | S3::clockMask |
        L4::clockMask | S4::clockMask | L5::clockMask | S5::clockMask |
        L6::clockMask | S6::clockMask | L7::clockMask | S7::clockMask |
        L8::clockMask | S8::clockMask>::apply();

    csl::PortConfiguration<csl::port::Id::a, S1::configuration,
                           S2::configuration, S3::configuration,
                           S4::configuration>::apply();
    csl::PortConfiguration<csl::port::Id::b, L1::configuration,
                           L2::configuration, L3::configuration,
                           L4::configuration, L5::configuration,
                           L6::configuration, L0::configuration>::apply();
    csl::PortConfiguration<csl::port::Id::c, S5::configuration,
                           S6::configuration, S0::configuration>::apply();
    csl::PortConfiguration<csl::port::Id::d, S7::configuration>::apply();
    csl::PortConfiguration<csl::port::Id::e, L7::configuration,
                           S8::configuration>::apply();
    csl::PortConfiguration<csl::port::Id::h, L8::configuration>::apply();
  }
};

}  // namespace bsl
#endif
