# CT-HAL23

Modern C++23 variant of the compile-time GPIO HAL.

Requires a C++23-capable compiler (`-std=c++23`). The legacy IAR project was
intentionally not copied because the historical toolchain in this repository
predates the required language features.

Modernized language features used:

- C++20 structural class non-type template parameters (`PinConfig` values)
- C++17 fold expressions instead of recursive `CombinedConfiguration<Head, Tail...>`
- `if constexpr` for compile-time removal of unused register updates
- `consteval` mask/value calculations and uniqueness validation
- C++20 Concepts / `requires` for supported ports and valid pin configurations
- `inline static constexpr` constants
- `[[no_unique_address]]` for empty stateless pin members
- C++23 `std::to_underlying` for enum conversion
- scoped enums (`enum class`) for stronger type safety

The hardware access and register layout checks remain equivalent to CPPCTHAL.
