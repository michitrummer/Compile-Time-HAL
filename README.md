# Compile-Time HAL

This repository demonstrates how fixed hardware mappings and GPIO configurations can be resolved at compile time in C++. Its focus is **CPPCTHAL**: ports, pins, and operating modes are described through template parameters and traits. The compiler can derive register addresses, masks, and values from these descriptions and optimize the access layers down to the hardware access.

The Compile-Time HAL concept and implementation were developed by **Michael Trummer**. The repository also includes a C++23 variant and other HAL implementations for comparison on the STM32L433.

The [manuscript for the ESE Kongress 2026 proceedings](docs/Manuscript.md) explains the approach through LED and button access, precomputed register values, and the combined configuration of multiple pins on a port.

## CPPCTHAL example

```cpp
#include <bsl/include/BoardConfig.h>
#include <bsl/include/Led.h>

int main() {
  bsl::BoardConfig::apply();

  bsl::Led<bsl::LedId::ld4> ld4;
  ld4.on();

  while (true) {
  }
}
```

`LedTraits` maps `bsl::LedId::ld4` to output PB13. The application uses the logical device identifier; the port and pin are fixed in the concrete type. `BoardConfig::apply()` enables the GPIO clocks and writes the precomputed configuration to the registers. The register accesses themselves take place at runtime.

The approach consists of three main steps:

- **Device mapping:** Traits associate logical devices with fixed pins and their configuration.
- **Configuration:** Templates calculate the required register masks and values at compile time.
- **Aggregation:** The configurations of multiple pins on a port are combined, with checks for port assignment and duplicate pin configurations.

Fixed mappings make it easier for the compiler to optimize access. The result still depends on optimization settings and whether the implementation is visible to the compiler. Changing the hardware mapping requires a new build.

## Implementations

| Directory | Approach |
| --- | --- |
| [CPPCTHAL](CPPCTHAL/) | Compile-Time HAL with templates, traits, and precomputed GPIO configuration |
| [CPPCTHAL23](CPPCTHAL23/) | Modernized variant using C++23 and additional language features from C++17 and C++20 |
| [CHAL](CHAL/) | C HAL for comparison |
| [CPPHAL](CPPHAL/) | Conventional C++ HAL for comparison |
| [CubeMXHAL](CubeMXHAL/) | STM32Cube HAL |
| [CubeMXLL](CubeMXLL/) | STM32Cube LL |
| [NoHAL](NoHAL/) | Direct register access |

CPPCTHAL implements the approach using C++11 language features; its current CMake targets use C++17. CPPCTHAL23 uses features including fold expressions, `if constexpr`, `consteval`, concepts, and `std::to_underlying`. See the [CPPCTHAL23 description](CPPCTHAL23/README_CTHAL23.md) for details.

## Building

The existing ARM GCC presets require:

- CMake **3.24 or later**, because the presets use [schema version 5](https://cmake.org/cmake/help/v3.24/manual/cmake-presets.7.html#format).
- Ninja.
- The GNU Arm Embedded Toolchain, with `arm-none-eabi-gcc`, `arm-none-eabi-g++`, and the corresponding binutils available in `PATH`.
- Newlib and the Arm C++ standard library. For CPPCTHAL23, the compiler and standard library must support the C++23 features used, including `std::to_underlying`.

The repository includes a [Dev Container](.devcontainer/devcontainer.json) for VS Code with the Arm toolchain. It requires Docker and the VS Code Dev Containers extension.

Clone the repository, then configure and build from its root directory:

```sh
git clone https://github.com/michitrummer/Compile-Time-HAL.git
cd Compile-Time-HAL

cmake --preset arm-gcc-release
cmake --build --preset arm-gcc-release --parallel
```

For a debug build, use `arm-gcc-debug` in both commands. The full build includes seven firmware targets and 14 benchmark targets for the two example scenarios.

After configuration, you can also build CPPCTHAL alone:

```sh
cmake --build --preset arm-gcc-release --target CPPCTHAL
```

Build outputs are placed in `build/arm-gcc-release/` or `build/arm-gcc-debug/`. In addition to ELF files, HEX and BIN files are generated when `arm-none-eabi-objcopy` is available. The target hardware is the STM32L433 with a Cortex-M4 core.

## Comparison examples and analysis

The [examples](Examples/README.md) perform the same basic task: read a button and control the corresponding LED output.

- [Single](Examples/Single/): one B1/LD4 pair.
- [MultiCall](Examples/MultiCall/README.md): the B1/LD4 pair plus eight additional example pairs with different pins and configurations.

The identifiers `ex1` through `ex8` are benchmark mappings for additional example connections.

The analysis also requires Bash and Python 3:

```sh
bash MemoryAnalysis.sh
```

[MemoryAnalysis.sh](MemoryAnalysis.sh) builds isolated benchmark ELFs and complete firmware ELFs. [MemoryAnalysisResults.py](MemoryAnalysisResults.py) evaluates memory usage and instruction counts; results are written to `MemoryAnalysis/`.

Results depend on the toolchain, optimization settings, and scenario. An instruction count is not a runtime measurement. The manuscript provides context for its comparison results and the revision used.

## License

Project files marked with `SPDX-License-Identifier: Apache-2.0` are licensed under the [Apache License 2.0](LICENSE). Bundled or generated STMicroelectronics files are subject to their respective license notices. See [LICENSE_SCOPE.md](LICENSE_SCOPE.md) and [NOTICE](NOTICE) for details.
