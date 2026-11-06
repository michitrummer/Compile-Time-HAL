#!/usr/bin/env bash
# Compare linked ARM instructions and flash sizes between commits before/after pin refactor.
set -euo pipefail
REPO="$(cd "${1:-.}" && pwd)"
BEFORE=d5eef707aee3017273098378c4de112d151711e4
AFTER=45a716557e6bdf413f713fc8f8abced7185a4a81
for cmd in git cmake arm-none-eabi-gcc arm-none-eabi-g++ arm-none-eabi-objdump arm-none-eabi-size python3; do
  command -v "$cmd" >/dev/null || { echo "Missing: $cmd" >&2; exit 1; }
done
git -C "$REPO" rev-parse --is-inside-work-tree >/dev/null
TEMP="$(mktemp -d)"
cleanup() {
  for name in before after; do
    if [[ -d "$TEMP/$name" ]]; then git -C "$REPO" worktree remove --force "$TEMP/$name" >/dev/null 2>&1 || true; fi
  done
  rm -rf "$TEMP"
}
trap cleanup EXIT
printf "commit,configuration,target,flash_bytes,instructions\n"
for entry in "before:$BEFORE" "after:$AFTER"; do
  label="${entry%%:*}"
  sha="${entry#*:}"
  wd="$TEMP/$label"
  git -C "$REPO" worktree add --detach "$wd" "$sha" >/dev/null 2>&1
  for config in Release Debug; do
    build="$wd/build-$config"
    cmake -S "$wd" -B "$build" -DCMAKE_BUILD_TYPE="$config" \
      -DCMAKE_TOOLCHAIN_FILE="$wd/cmake/toolchains/arm-none-eabi-gcc.cmake" \
      >"$TEMP/configure-${label}-${config}.log"
    cmake --build "$build" --parallel 2 --target \
      CPPCTHAL cppcthal_single cppcthal_multicall \
      CPPCTHAL23 cppcthal23_single cppcthal23_multicall \
      >"$TEMP/build-${label}-${config}.log"
    for target in CPPCTHAL CPPCTHAL23 cppcthal_single cppcthal_multicall cppcthal23_single cppcthal23_multicall; do
      case "$target" in
        CPPCTHAL|cppcthal_*) subdir=CPPCTHAL ;;
        CPPCTHAL23|cppcthal23_*) subdir=CPPCTHAL23 ;;
        *) echo "Unexpected target: $target" >&2; exit 2 ;;
      esac
      elf="$build/$subdir/$target.elf"
      arm-none-eabi-size -B "$elf" | \
        awk -v rev="$label" -v cfg="$config" -v name="$target" \
          'NR==2 {printf "%s,%s,%s,%d,",rev,cfg,name,$1+$2}'
      arm-none-eabi-objdump -d "$elf" | python3 -c '
import re, sys
pattern = re.compile(r"^\s*[0-9a-f]+:\s+(?:[0-9a-f]{4}\s+){1,2}([.a-z][.a-z0-9]*)", re.I)
count = 0
for line in sys.stdin:
    m = pattern.match(line)
    if m and m.group(1).lower() not in {".word", ".short", ".byte", ".inst", ".inst.n", ".inst.w"}:
        count += 1
print(count)
'
    done
  done
done
