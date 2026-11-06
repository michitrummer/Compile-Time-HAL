# Memory analysis and reproducibility

This workflow documents the analysis infrastructure and HAL variants at source
revision [`45a7165`](https://github.com/michitrummer/Compile-Time-HAL/tree/45a716557e6bdf413f713fc8f8abced7185a4a81).
The provenance additions are subsequent changes. A run records its actual `HEAD`,
not a hard-coded `45a7165`. Existing slide values have not been remeasured by this
documentation update.

## Run the analysis

Use Bash, Python 3, Git, CMake, a CMake-supported build tool, and GNU Arm GCC/G++
with newlib and C++23 library support (including `std::to_underlying`). The
repository's Dev Container supplies these dependencies. Its compiler packages
are not version-pinned; consult the recorded version rather than inferring one
from the container image.

Run from a checkout of the revision you want to measure:

```sh
bash MemoryAnalysis.sh
```

The script replaces the generated `MemoryAnalysis/` directory. Archive any
previous results first. Keep the checkout unchanged while the script runs.
Prefer a clean checkout for published measurements. Local tracked or untracked
changes are recorded as `working_tree_dirty: true` (excluding the output
directory and Git-ignored files). Such a run cannot be identified by its commit
alone; preserve the modified sources separately.

To select another installed GNU Arm toolchain, set its executable prefix:

```sh
ARM_GCC_PREFIX=/opt/arm-toolchain/bin/arm-none-eabi bash MemoryAnalysis.sh
```

The same prefix is passed to the isolated builds and CMake. Both C and C++
compiler paths and version strings are recorded before compilation. Builds
require the full toolchain, including the binutils tools and libraries.

## What is built

Both `Single` and `MultiCall` cover NoHAL, CHAL, CPPHAL, CPPCTHAL, CPPCTHAL23,
STM32Cube HAL, and STM32Cube LL: 14 isolated ELFs and 14 complete firmware ELFs.
MultiCall uses LD4/B1 plus eight benchmark-only LED/switch pairs with distinct
hardware mappings; it is not nine accesses to the same pin pair.

| Setting | Isolated benchmark | Complete firmware benchmark |
| --- | --- | --- |
| Target | STM32L433, Cortex-M4, Thumb | Same |
| Optimization | `-O3 -fno-unroll-loops` | CMake Release, benchmark targets use `-O3` |
| C++ dialect | `gnu++17`; CPPCTHAL23 uses `gnu++23` | C++17; CPPCTHAL23 uses C++23, extensions disabled |
| C++ options | `-fno-exceptions -fno-rtti -fno-threadsafe-statics` | Same |
| Sections | `-ffunction-sections -fdata-sections`, linker `--gc-sections` | Same |
| Startup/linking | Generated minimal linker script, entry `main`, direct `ld` | Target startup code and firmware linker script |

The isolated build also uses `-ffreestanding -fno-builtin -fstack-usage
-fno-unwind-tables -fno-asynchronous-unwind-tables`. These options must not be
assumed for the full build. There is no explicit LTO option in this workflow.
The recorded commands are authoritative for each run, including defines,
include paths, language standards, and additional CMake flags.

## Output files

| File under `MemoryAnalysis/` | Content |
| --- | --- |
| `build-metadata.json` | Start time (UTC), source commit, dirty status, tool versions/paths, shared isolated flags |
| `compiler.txt`, `cxx_compiler.txt`, `linker.txt` | Full tool version output |
| `build-commands.log` | Shell-escaped isolated compile/link commands and CMake invocations |
| `full-build/compile_commands.json` | Per-file CMake compiler commands |
| `full-build.log` | Verbose full-build output, including link commands |
| `metadata.csv` | Provenance and target-capacity summary |
| `results.csv` | One row per scenario and implementation |
| `results.json` | The metadata and all result rows together |
| `isolated/`, `full-build/` | ELFs, maps, objects and other build artifacts |

Only a successfully completed run produces a complete new result set. A failed
build may leave partial logs and provenance; do not publish it as a measurement.
The results reader expects `build-metadata.json` from the shell script.

## Meaning of the metrics

- `main_code_bytes`: size of `main` in the isolated ELF.
- `isolated_flash_bytes`: isolated ELF text plus data image.
- `full_flash_bytes`: complete firmware text plus data image, including startup.
- `loop_instructions`: static instructions between `benchmark_loop_start` and
  `benchmark_loop_end` in the isolated ELF. Called functions are excluded.
- `expanded_instructions`: that count plus the instruction bodies of resolved
  direct callees for each call site. Expansion is one level deep; it does not
  simulate branch paths or recursively count nested calls.
- `full_ram_bytes`: data plus BSS, including linker heap/stack reservations.
  This is not peak runtime RAM usage. `main_stack_bytes` describes only `main`.

Instruction counts are static code metrics, not executed instruction counts or
timings. When comparing HALs with calls, state which count is shown. Do not mix
isolated and complete-program sizes without labeling them.

## Attribution in slides and reports

Use the actual values in `metadata.csv` or `results.json`: source commit,
`working_tree_dirty`, compiler versions, target, and scenario. State that loop
counts come from the isolated `-O3 -fno-unroll-loops` build and total flash from
the complete firmware `-O3` build. Keep the logs with the published results.

For Slide 28, use MultiCall `main_code_bytes`, `full_flash_bytes`, and an explicitly
labeled instruction metric from the same run. Include CPPCTHAL23 if showing all
seven implementations, or state that the chart selects six variants. Replace
the chart values and recalculate percentages from that run before attaching its
commit/compiler attribution. The older manuscript table is a historical source,
not evidence of a run at `45a7165`.
