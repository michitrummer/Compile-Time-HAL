#!/usr/bin/env bash
# Created on: 2026-07-22
# Author: Michael Trummer
# Copyright 2026 Michael Trummer
# SPDX-License-Identifier: Apache-2.0

set -euo pipefail

readonly ROOT="$(cd "$(dirname "$0")" && pwd)"
readonly OUT="$ROOT/MemoryAnalysis"
readonly OBJ="$OUT/isolated/obj"
readonly ELF="$OUT/isolated/elf"
readonly FULL_BUILD="$OUT/full-build"
readonly ARM_GCC_PREFIX="${ARM_GCC_PREFIX:-arm-none-eabi}"

readonly CC="${ARM_GCC_PREFIX}-gcc"
readonly CXX_COMPILER="${ARM_GCC_PREFIX}-g++"
readonly LD="${ARM_GCC_PREFIX}-ld"
readonly SIZE="${ARM_GCC_PREFIX}-size"
readonly NM="${ARM_GCC_PREFIX}-nm"
readonly OBJDUMP="${ARM_GCC_PREFIX}-objdump"

require_tool() {
  if ! command -v "$1" >/dev/null 2>&1; then
    echo "Required tool not found: $1" >&2
    exit 1
  fi
}

for tool in \
  "$CC" "$CXX_COMPILER" "$LD" "$SIZE" "$NM" "$OBJDUMP" \
  cmake python3 git; do
  require_tool "$tool"
done

rm -rf "$OUT"
mkdir -p "$OBJ" "$ELF"

cat >"$OUT/minimal.ld" <<'LDS'
/*
 * Generated minimal linker script for the benchmark-only ELF.
 * Created on: 2026-07-22
 * Author: Michael Trummer
 * Copyright 2026 Michael Trummer
 * SPDX-License-Identifier: Apache-2.0
 */

ENTRY(main)

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 256K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.text.main))
    *(.text .text.*)
  } > FLASH

  .rodata :
  {
    *(.rodata .rodata.*)
  } > FLASH

  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > FLASH
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > FLASH
  .init_array : { KEEP(*(.init_array .init_array.*)) } > FLASH
  .fini_array : { KEEP(*(.fini_array .fini_array.*)) } > FLASH

  .data :
  {
    *(.data .data.*)
  } > RAM AT > FLASH

  .bss (NOLOAD) :
  {
    *(.bss .bss.*)
    *(COMMON)
  } > RAM

  /DISCARD/ :
  {
    *(.comment)
    *(.note*)
  }
}
LDS

readonly COMMON=(
  -mcpu=cortex-m4
  -mthumb
  -ffreestanding
  -fno-builtin
  -O3
  -fno-unroll-loops
  -DSTM32L433xx
  -ffunction-sections
  -fdata-sections
  -fstack-usage
  -fno-unwind-tables
  -fno-asynchronous-unwind-tables
  -c
)
readonly CXX_FLAGS=(
  -fno-exceptions
  -fno-rtti
  -fno-threadsafe-statics
)
readonly HAL_INC=(
  -I"$ROOT/CubeMXHAL"
  -I"$ROOT/CubeMXHAL/Inc"
  -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Include"
  -I"$ROOT/CubeMXHAL/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
  -I"$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Inc"
)
readonly LL_INC=(
  -I"$ROOT/CubeMXLL"
  -I"$ROOT/CubeMXLL/Inc"
  -I"$ROOT/CubeMXLL/Drivers/CMSIS/Include"
  -I"$ROOT/CubeMXLL/Drivers/CMSIS/Device/ST/STM32L4xx/Include"
  -I"$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Inc"
)

compile_c() {
  local output="$1"
  shift
  "$CC" "${COMMON[@]}" "$@" -o "$output"
}

compile_cpp() {
  local output="$1"
  local standard="$2"
  shift 2
  "$CXX_COMPILER" \
    "${COMMON[@]}" \
    "${CXX_FLAGS[@]}" \
    "-std=$standard" \
    "$@" \
    -o "$output"
}

link_isolated_elf() {
  local output="$1"
  shift
  "$LD" \
    -T "$OUT/minimal.ld" \
    -e main \
    --gc-sections \
    "-Map=${output%.elf}.map" \
    -o "$output" \
    "$@"
}

echo "Building 14 isolated benchmark ELFs ..."
for example in Single MultiCall; do
  readonly_source="$ROOT/Examples/$example"
  example_key="${example,,}"

  for implementation in \
    nohal chal cpphal cppcthal cppcthal23 cubemxhal cubemxll; do
    object_dir="$OBJ/$example_key/$implementation"
    elf_dir="$ELF/$example_key"
    mkdir -p "$object_dir" "$elf_dir"
    objects=()

    case "$implementation" in
      nohal)
        compile_c \
          "$object_dir/main.o" \
          -I"$ROOT/NoHAL" \
          -I"$ROOT/NoHAL/Src" \
          "$readonly_source/nohal.c"
        objects+=("$object_dir/main.o")
        ;;
      chal)
        compile_c \
          "$object_dir/main.o" \
          -I"$ROOT/CHAL" \
          "$readonly_source/chal.c"
        objects+=("$object_dir/main.o")
        for unit in pin port rcc; do
          compile_c \
            "$object_dir/$unit.o" \
            -I"$ROOT/CHAL" \
            "$ROOT/CHAL/csl/src/$unit.c"
          objects+=("$object_dir/$unit.o")
        done
        ;;
      cpphal)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++17 \
          -I"$ROOT/CPPHAL" \
          "$readonly_source/cpphal.cpp"
        objects+=("$object_dir/main.o")
        for unit in Pin Port RCC; do
          compile_cpp \
            "$object_dir/$unit.o" \
            gnu++17 \
            -I"$ROOT/CPPHAL" \
            "$ROOT/CPPHAL/csl/src/$unit.cpp"
          objects+=("$object_dir/$unit.o")
        done
        ;;
      cppcthal)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++17 \
          -I"$ROOT/CPPCTHAL" \
          "$readonly_source/cppcthal.cpp"
        objects+=("$object_dir/main.o")
        ;;
      cppcthal23)
        compile_cpp \
          "$object_dir/main.o" \
          gnu++23 \
          -I"$ROOT/CPPCTHAL23" \
          "$readonly_source/cppcthal23.cpp"
        objects+=("$object_dir/main.o")
        ;;
      cubemxhal)
        compile_c \
          "$object_dir/main.o" \
          -DUSE_HAL_DRIVER \
          "${HAL_INC[@]}" \
          "$readonly_source/cubemx-hal.c"
        compile_c \
          "$object_dir/gpio.o" \
          -DUSE_HAL_DRIVER \
          "${HAL_INC[@]}" \
          "$ROOT/CubeMXHAL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c"
        objects+=("$object_dir/main.o" "$object_dir/gpio.o")
        ;;
      cubemxll)
        compile_c \
          "$object_dir/main.o" \
          -DUSE_FULL_LL_DRIVER \
          "${LL_INC[@]}" \
          "$readonly_source/cubemx-ll.c"
        compile_c \
          "$object_dir/gpio.o" \
          -DUSE_FULL_LL_DRIVER \
          "${LL_INC[@]}" \
          "$ROOT/CubeMXLL/Drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_ll_gpio.c"
        objects+=("$object_dir/main.o" "$object_dir/gpio.o")
        ;;
    esac

    link_isolated_elf \
      "$elf_dir/$implementation.elf" \
      "${objects[@]}"
  done
done

"$CC" --version >"$OUT/compiler.txt"

echo "Building 14 complete STM32 firmware ELFs ..."
cmake \
  -S "$ROOT" \
  -B "$FULL_BUILD" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE="$ROOT/cmake/toolchains/arm-none-eabi-gcc.cmake"

full_targets=()
for example_key in single multicall; do
  for implementation in \
    nohal chal cpphal cppcthal cppcthal23 cubemxhal cubemxll; do
    full_targets+=("${implementation}_${example_key}")
  done
done
cmake \
  --build "$FULL_BUILD" \
  --parallel \
  --target "${full_targets[@]}"

echo "Extracting instruction, memory and section values ..."
python3 - \
  "$ROOT" \
  "$OUT" \
  "$FULL_BUILD" \
  "$SIZE" \
  "$NM" \
  "$OBJDUMP" \
  "$CC" <<'PY'
from __future__ import annotations

import csv
import json
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path


ROOT = Path(sys.argv[1])
OUT = Path(sys.argv[2])
FULL_BUILD = Path(sys.argv[3])
SIZE = sys.argv[4]
NM = sys.argv[5]
OBJDUMP = sys.argv[6]
CC = sys.argv[7]
ISOLATED = OUT / "isolated"

IMPLEMENTATIONS = [
    ("NoHAL", "nohal", "NoHAL"),
    ("CHAL", "chal", "CHAL"),
    ("CPPHAL", "cpphal", "CPPHAL"),
    ("CPPCTHAL", "cppcthal", "CPPCTHAL"),
    ("CPPCTHAL23", "cppcthal23", "CPPCTHAL23"),
    ("CubeMX HAL", "cubemxhal", "CubeMXHAL"),
    ("CubeMX LL", "cubemxll", "CubeMXLL"),
]
EXAMPLES = [
    ("Single", "single"),
    ("MultiCall", "multicall"),
]

RESULT_FIELDS = [
    "example",
    "implementation",
    "loop_instructions",
    "loop_code_bytes",
    "direct_calls",
    "expanded_instructions",
    "call_targets",
    "main_code_bytes",
    "isolated_text_bytes",
    "isolated_data_bytes",
    "isolated_bss_bytes",
    "isolated_flash_bytes",
    "extra_flash_outside_main_bytes",
    "isolated_static_ram_bytes",
    "main_stack_bytes",
    "full_text_bytes",
    "full_data_bytes",
    "full_bss_including_reservation_bytes",
    "full_flash_bytes",
    "full_ram_bytes",
    "physical_total_bytes",
    "platform_flash_overhead_bytes",
    "platform_ram_overhead_bytes",
    "flash_capacity_bytes",
    "ram_capacity_bytes",
    "flash_usage",
    "ram_usage",
    "section_isr_vector_bytes",
    "section_code_rodata_bytes",
    "section_init_array_bytes",
    "section_fini_array_bytes",
    "section_data_image_bytes",
    "section_runtime_bss_bytes",
    "heap_reservation_bytes",
    "stack_reservation_bytes",
    "heap_stack_section_bytes",
    "reservation_delta_bytes",
    "section_flash_delta_bytes",
    "section_ram_delta_bytes",
    "isolated_elf",
    "firmware_elf",
]


def run(*args: str) -> str:
    return subprocess.check_output(args, text=True)


def parse_size(elf: Path) -> dict[str, int]:
    fields = run(SIZE, str(elf)).strip().splitlines()[-1].split()
    return {
        "text": int(fields[0]),
        "data": int(fields[1]),
        "bss": int(fields[2]),
    }


def parse_sections(elf: Path) -> dict[str, int]:
    sections: dict[str, int] = {}
    for line in run(SIZE, "-A", "-d", str(elf)).splitlines():
        match = re.match(
            r"^(\.[A-Za-z0-9_]+)\s+(\d+)\s+\d+$",
            line.strip(),
        )
        if match:
            sections[match.group(1)] = int(match.group(2))
    return sections


def parse_symbols(elf: Path) -> list[tuple[int, int, str, str]]:
    symbols: list[tuple[int, int, str, str]] = []
    output = run(
        NM,
        "-n",
        "-S",
        "--defined-only",
        "-C",
        str(elf),
    )
    for line in output.splitlines():
        match = re.match(
            r"^([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+(\S)\s+(.+)$",
            line,
        )
        if match:
            symbols.append(
                (
                    int(match.group(1), 16),
                    int(match.group(2), 16),
                    match.group(3),
                    match.group(4),
                )
            )
            continue

        match = re.match(
            r"^([0-9a-fA-F]+)\s+(\S)\s+(.+)$",
            line,
        )
        if match:
            symbols.append(
                (
                    int(match.group(1), 16),
                    0,
                    match.group(2),
                    match.group(3),
                )
            )
    return symbols


def parse_disassembly(elf: Path) -> list[dict[str, int | str]]:
    instruction_re = re.compile(
        r"^\s*([0-9a-fA-F]+):\s+"
        r"(?:(?:[0-9a-fA-F]{4}|[0-9a-fA-F]{8})\s+)+"
        r"([A-Za-z0-9_.]+)\s*(.*)$"
    )
    instructions: list[dict[str, int | str]] = []
    for line in run(OBJDUMP, "-d", "-C", str(elf)).splitlines():
        match = instruction_re.match(line)
        if not match or match.group(2).startswith("."):
            continue
        instructions.append(
            {
                "address": int(match.group(1), 16),
                "mnemonic": match.group(2),
                "operands": match.group(3),
            }
        )
    return instructions


def parse_main_stack(example_key: str, implementation_key: str) -> int:
    usage_file = (
        ISOLATED
        / "obj"
        / example_key
        / implementation_key
        / "main.su"
    )
    for line in usage_file.read_text().splitlines():
        fields = line.split("\t")
        if len(fields) < 2:
            continue
        function_name = fields[0].split(":", 3)[-1]
        if re.search(r"\bmain(?:\(\))?$", function_name):
            return int(fields[1])
    raise RuntimeError(f"main stack usage missing in {usage_file}")


def parse_capacity(memory_name: str) -> int:
    linker_script = (
        ROOT / "CPPCTHAL" / "stm32l433xc_flash.ld"
    ).read_text()
    match = re.search(
        rf"^\s*{memory_name}\b.*?\bLENGTH\s*=\s*"
        r"(0x[0-9a-fA-F]+|\d+)\s*([KMG]?)",
        linker_script,
        re.MULTILINE,
    )
    if not match:
        raise RuntimeError(
            f"Could not read {memory_name} capacity from linker script"
        )
    value = int(match.group(1), 0)
    multiplier = {
        "": 1,
        "K": 1024,
        "M": 1024**2,
        "G": 1024**3,
    }[match.group(2)]
    return value * multiplier


def relative(path: Path) -> str:
    return str(path.relative_to(ROOT))


flash_capacity = parse_capacity("FLASH")
ram_capacity = parse_capacity("RAM")

first_full_elf = (
    FULL_BUILD / "NoHAL" / "nohal_single.elf"
)
first_full_symbols = {
    name: address
    for address, _, _, name in parse_symbols(first_full_elf)
}
heap_reservation = first_full_symbols["_Min_Heap_Size"]
stack_reservation = first_full_symbols["_Min_Stack_Size"]

rows: list[dict[str, int | float | str]] = []
for example_name, example_key in EXAMPLES:
    for implementation_name, implementation_key, cmake_dir in IMPLEMENTATIONS:
        isolated_elf = (
            ISOLATED
            / "elf"
            / example_key
            / f"{implementation_key}.elf"
        )
        firmware_elf = (
            FULL_BUILD
            / cmake_dir
            / f"{implementation_key}_{example_key}.elf"
        )

        isolated_size = parse_size(isolated_elf)
        firmware_size = parse_size(firmware_elf)
        sections = parse_sections(firmware_elf)

        symbols = parse_symbols(isolated_elf)
        symbol_map = {
            name: (address, size)
            for address, size, _, name in symbols
        }
        instructions = parse_disassembly(isolated_elf)

        loop_start = symbol_map["benchmark_loop_start"][0]
        loop_end = symbol_map["benchmark_loop_end"][0]
        loop = [
            instruction
            for instruction in instructions
            if loop_start
            <= int(instruction["address"])
            < loop_end
        ]
        calls = [
            instruction
            for instruction in loop
            if instruction["mnemonic"]
            in {"bl", "blx", "bl.w", "blx.w"}
        ]

        expanded_instructions = len(loop)
        call_target_names: list[str] = []
        for call in calls:
            match = re.search(r"<(.+)>", str(call["operands"]))
            if not match:
                continue
            target = match.group(1)
            call_target_names.append(target)
            target_address, target_size = symbol_map.get(target, (0, 0))
            expanded_instructions += sum(
                1
                for instruction in instructions
                if target_address
                <= int(instruction["address"])
                < target_address + target_size
            )

        target_counts = Counter(call_target_names)
        call_targets = "; ".join(
            f"{target} x {count}"
            for target, count in target_counts.items()
        )

        isolated_flash = (
            isolated_size["text"] + isolated_size["data"]
        )
        isolated_static_ram = (
            isolated_size["data"] + isolated_size["bss"]
        )
        full_flash = firmware_size["text"] + firmware_size["data"]
        full_ram = firmware_size["data"] + firmware_size["bss"]

        section_isr_vector = sections.get(".isr_vector", 0)
        section_code_rodata = sum(
            sections.get(name, 0)
            for name in (
                ".text",
                ".rodata",
                ".ARM.extab",
                ".ARM",
                ".preinit_array",
            )
        )
        section_init_array = sections.get(".init_array", 0)
        section_fini_array = sections.get(".fini_array", 0)
        section_data_image = (
            sections.get(".data", 0) + sections.get(".tdata", 0)
        )
        section_runtime_bss = (
            sections.get(".bss", 0) + sections.get(".tbss", 0)
        )
        heap_stack_section = sections.get("._user_heap_stack", 0)

        section_flash = (
            section_isr_vector
            + section_code_rodata
            + section_init_array
            + section_fini_array
            + section_data_image
        )
        section_ram = (
            section_data_image
            + section_runtime_bss
            + heap_stack_section
        )

        row: dict[str, int | float | str] = {
            "example": example_name,
            "implementation": implementation_name,
            "loop_instructions": len(loop),
            "loop_code_bytes": loop_end - loop_start,
            "direct_calls": len(calls),
            "expanded_instructions": expanded_instructions,
            "call_targets": call_targets or "-",
            "main_code_bytes": symbol_map["main"][1],
            "isolated_text_bytes": isolated_size["text"],
            "isolated_data_bytes": isolated_size["data"],
            "isolated_bss_bytes": isolated_size["bss"],
            "isolated_flash_bytes": isolated_flash,
            "extra_flash_outside_main_bytes": (
                isolated_flash - symbol_map["main"][1]
            ),
            "isolated_static_ram_bytes": isolated_static_ram,
            "main_stack_bytes": parse_main_stack(
                example_key,
                implementation_key,
            ),
            "full_text_bytes": firmware_size["text"],
            "full_data_bytes": firmware_size["data"],
            "full_bss_including_reservation_bytes": firmware_size["bss"],
            "full_flash_bytes": full_flash,
            "full_ram_bytes": full_ram,
            "physical_total_bytes": full_flash + full_ram,
            "platform_flash_overhead_bytes": (
                full_flash - isolated_flash
            ),
            "platform_ram_overhead_bytes": (
                full_ram - isolated_static_ram
            ),
            "flash_capacity_bytes": flash_capacity,
            "ram_capacity_bytes": ram_capacity,
            "flash_usage": full_flash / flash_capacity,
            "ram_usage": full_ram / ram_capacity,
            "section_isr_vector_bytes": section_isr_vector,
            "section_code_rodata_bytes": section_code_rodata,
            "section_init_array_bytes": section_init_array,
            "section_fini_array_bytes": section_fini_array,
            "section_data_image_bytes": section_data_image,
            "section_runtime_bss_bytes": section_runtime_bss,
            "heap_reservation_bytes": heap_reservation,
            "stack_reservation_bytes": stack_reservation,
            "heap_stack_section_bytes": heap_stack_section,
            "reservation_delta_bytes": (
                heap_stack_section
                - heap_reservation
                - stack_reservation
            ),
            "section_flash_delta_bytes": section_flash - full_flash,
            "section_ram_delta_bytes": section_ram - full_ram,
            "isolated_elf": relative(isolated_elf),
            "firmware_elf": relative(firmware_elf),
        }

        for delta_name in (
            "reservation_delta_bytes",
            "section_flash_delta_bytes",
            "section_ram_delta_bytes",
        ):
            if row[delta_name] != 0:
                raise RuntimeError(
                    f"{example_name}/{implementation_name}: "
                    f"{delta_name}={row[delta_name]}"
                )
        rows.append(row)

compiler = run(CC, "--version").splitlines()[0]
commit = run(
    "git",
    "-C",
    str(ROOT),
    "rev-parse",
    "HEAD",
).strip()
metadata = [
    ("repository", "michitrummer/Compile-Time-HAL", ""),
    ("commit", commit, ""),
    ("compiler", compiler, ""),
    ("target", "STM32L433 / Cortex-M4 / Thumb", ""),
    ("isolated_optimization", "-O3 -fno-unroll-loops", ""),
    (
        "full_build",
        "CMake Release; benchmark targets use -O3",
        "",
    ),
    ("flash_capacity", flash_capacity, "B"),
    ("ram_capacity", ram_capacity, "B"),
    ("heap_reservation", heap_reservation, "B"),
    ("stack_reservation", stack_reservation, "B"),
]

with (OUT / "results.csv").open("w", newline="") as output_file:
    writer = csv.DictWriter(output_file, fieldnames=RESULT_FIELDS)
    writer.writeheader()
    writer.writerows(rows)

with (OUT / "metadata.csv").open("w", newline="") as output_file:
    writer = csv.writer(output_file)
    writer.writerow(("name", "value", "unit"))
    writer.writerows(metadata)

with (OUT / "results.json").open("w") as output_file:
    json.dump(
        {
            "metadata": {
                name: value
                for name, value, _ in metadata
            },
            "rows": rows,
        },
        output_file,
        indent=2,
    )
    output_file.write("\n")

print(f"Wrote {len(rows)} rows to {OUT / 'results.csv'}")
print(f"Wrote metadata to {OUT / 'metadata.csv'}")
print(f"Wrote machine-readable data to {OUT / 'results.json'}")
PY

echo "Analysis completed successfully."
