/**
 * @file BindOnly.h
 * @brief Tag type selecting binding without per-object GPIO configuration.
 *
 * Created on: 2026-07-22
 * Author: Michael Trummer
 *
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BSL_BIND_ONLY_H_
#define BSL_BIND_ONLY_H_
namespace bsl
{
/** @brief Constructor tag selecting GPIO binding without configuration. */
struct BindOnly { };

/** @brief Reusable instance of the bind-only constructor tag. */
static constexpr BindOnly bindOnly = BindOnly();
}
#endif
