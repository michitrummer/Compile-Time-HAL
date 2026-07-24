# CT-HAL23 modernization map

| Previous CPPCTHAL construct | CT-HAL23 replacement |
|---|---|
| `PinConfiguration<id, mode, pull, speed>` helper type | structural `PinConfig{...}` value NTTP |
| recursive `CombinedConfiguration<Head, Tail...>` | C++17 fold expressions |
| ordinary `if (constexpr-mask != 0)` | `if constexpr` |
| `static_cast<uint32_t>(enum)` | C++23 `std::to_underlying` |
| helper constant expressions | `consteval` functions |
| deep template errors for invalid config | Concepts / `requires` |
| duplicate-pin bit overlap via recursive template | `consteval` loop + `static_assert` |
| out-of-class/static constant patterns | `inline static constexpr` |
| empty Pin member | `[[no_unique_address]]` |
| unscoped GPIO enums | scoped `enum class` |

The expected runtime GPIO access is unchanged; the modernization primarily
simplifies source-level compile-time logic and diagnostics.
