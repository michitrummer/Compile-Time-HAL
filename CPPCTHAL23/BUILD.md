# Building CT-HAL23

CT-HAL23 requires a compiler with C++23 support. The benchmark build uses:

```text
clang++ 17
-std=c++23
-target arm-none-eabi
-mcpu=cortex-m4
-mthumb
-O3
```

Run the repository-level scripts to reproduce the comparison:

```sh
./InstructionAnalysis/build_analysis.sh
python3 InstructionAnalysis/count_instructions.py
./MemoryAnalysis/build_memory.sh
```
