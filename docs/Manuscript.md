# Compile-Time HAL

**Resolving fixed hardware configurations at compile time with C++**

Michael Trummer, IMT Information Management Technology AG

**A Compile-Time HAL represents fixed hardware mappings in C++ types and calculates configuration values during compilation. Using CPPCTHAL, an implementation based on C++11 language features, this paper shows how template parameters and traits define device mappings, generate register values, and combine multiple pin descriptions. A comparison examines code size and execution overhead; CPPCTHAL23 provides an outlook on newer language features.**

## Fixed connections as the starting point

For an LED with a fixed connection, the port address and bit mask are known. Many HAL interfaces nevertheless determine them from a device number on every access, for example through a lookup table or different code branches. This introduces additional memory accesses or branches for a mapping that does not change. Hardware parameters stored in handles also occupy RAM. CPPCTHAL defines the mapping in the type, allowing the compiler to reduce access to a fixed register target.

CPPCTHAL describes `Port`, `Pin`, and operating mode through template parameters. The selected hardware therefore becomes part of the concrete C++ type. For that type, the compiler can derive register addresses, bit masks, and configuration values. Hardware access remains a runtime operation; device selection and the calculation of fixed values take place at compile time.

CPPCTHAL [1] implements the approach for the STM32L433 using C++11 language features. The complete source code, including the comparison projects, is available in the following repository:

[https://github.com/michitrummer/Compile-Time-HAL](https://github.com/michitrummer/Compile-Time-HAL)

## Selecting LEDs and buttons through template parameters

In this example, `LD4` and `B1` are the logical device identifiers for the LED on `PB13` and the button on `PC13`. Traits map these identifiers to the concrete `Pin` type and its configuration: `PB13` as an output, `PC13` as an input. For a different board, this central mapping is adjusted; the application logic can retain its device names.

Listing 1 shows the application logic in `main`. `bsl` denotes the board layer. `Led<bsl::LedId::ld4>` and `Switch<bsl::SwitchId::b1>` instantiate device types for the fixed device selection. The selection is encoded in the type and resolved at compile time. As a result, calls to `on()`, `off()`, and `pressed()` do not need branching logic to select a device.

```cpp
#include <bsl/include/BoardConfig.h>
#include <bsl/include/Led.h>
#include <bsl/include/Switch.h>

int main() {
  bsl::BoardConfig::apply();
  bsl::Led<bsl::LedId::ld4> led;
  bsl::Switch<bsl::SwitchId::b1> sw;

  while (true) {
    if (sw.pressed())
      led.on();
    else
      led.off();
  }
}
```

Listing 1: Application with a fixed LED and button mapping. Source: simplified from [1], `CPPCTHAL/Src/main.cpp`.

`BoardConfig::apply()` enables the GPIO clocks and configures the pins. The loop in `main` then processes the button state and controls the LED. This part of the application logic uses logical devices; concrete register addresses and bit masks are defined in the HAL and known at compile time. The generated program uses them for hardware access.

## Deriving register values from template parameters

The next step is to derive register values from the `Pin` type. In CPPCTHAL, a configuration class performs this calculation using template parameters for the pin number, operating mode, and other settings. Listing 2 reduces this class to the mode field in `MODER`; the calculation follows the principle used in CPPCTHAL.

For the LED, pin 13 and output mode are already specified at compile time. From these parameters, the configuration class produces the `MODER` mask `0x0C000000` and the value `0x04000000`. No runtime function is needed to calculate these values for the specific pin.

`ModeConfig` represents the mode portion of a pin description. `LedMode` is the specialization for pin 13 as a push-pull output. The example values in the comments apply to this specialization.

```cpp
#include <cstdint>
#include <csl/include/Pin.h>

template <csl::pin::Id Id,
          csl::pin::Mode Mode>
struct ModeConfig {
  enum : uint32_t {
    shift = static_cast<uint32_t>(Id) * 2u,
    mask = 3u << shift,
    value =
        (static_cast<uint32_t>(Mode) & 3u)
        << shift
  };
};

using LedMode = ModeConfig<
    csl::pin::pin13, csl::pin::outPP>;
// Example values for this specialization:
// mask = 0x0C000000u, value = 0x04000000u
```

Listing 2: Register constants derived from the pin number and operating mode. Source: representation reduced to `MODER` from [1], `CPPCTHAL/csl/include/GpioConfig.h`.

The `enum` constants are produced when `ModeConfig` is instantiated from the template arguments `Id` and `Mode`. They do not form mutable configuration state. CPPCTHAL also uses this pattern for output type, speed, and pull resistors.

Initialization then uses the precomputed masks and values and writes them to the registers at runtime. Compile-time configuration therefore does not mean that hardware is configured during compilation: the compiler produces the constants, and the initialization code uses them when it runs.

## Preparing multiple pins together

Listing 3 adds `EX1`, an additional example output on `PB0`, to the LED `LD4` on `PB13`. The corresponding `LedTraits` define `EX1` as a push-pull output. Both pins belong to port B and can be configured together.

```cpp
#include <bsl/include/Led.h>
#include <csl/include/GpioConfig.h>

using LD4 = bsl::LedTraits<bsl::LedId::ld4>;  // PB13
using EX1 = bsl::LedTraits<bsl::LedId::ex1>;  // PB0

using PortB = csl::PortConfiguration<
    csl::port::b, LD4, EX1>;

void configurePortB() {
  csl::GpioClockConfiguration<
      LD4::clockMask | EX1::clockMask>::apply();
  PortB::apply();
}
```

Listing 3: Configuring two pins together for port B. Source: simplified from [1], `CPPCTHAL/bsl/include/BoardConfig.h` and `Led.h`.

`PortConfiguration` takes the port and the traits types as template arguments. The variadic template accepts multiple pin descriptions. CPPCTHAL combines their masks and values recursively at compile time. `configurePortB()` first enables the shared `GPIOB` clock, then writes the precomputed configuration to the registers through `PortB::apply()`.

During compilation, CPPCTHAL checks the port assignment and rejects duplicate configurations of the same pin. Separate port configurations are still required for the LED on `PB13` and the button on `PC13`; `BoardConfig::apply()` in Listing 1 handles them.

## Why the access layers can disappear

`on()` reaches the hardware access through the `Pin` type defined by the traits. The port and pin are fixed in every specialization involved. If the compiler can inline the small access functions defined in the headers, it can combine the layers into a register access with a constant target and write value.

This depends on optimization settings and code visibility. Conventional C interfaces can also be compiled efficiently when used with constants. CPPCTHAL explicitly exposes fixed hardware choices in the interface, making them available for checking.

## Comparing code size and execution overhead

The MultiCall scenario performs multiple LED and button accesses using different HAL variants [3]. Table 1 compares flash usage and the number of instructions in the main loop. Flash `main` refers to the code size of the `main` function; total flash covers the complete program.

| Variant | Flash `main` [bytes] | Total flash [bytes] | Instructions in main loop |
| --- | ---: | ---: | ---: |
| Macros / NoHAL | 1284 | 2148 | 46 |
| CHAL | 780 | 1644 | 63 |
| CPPHAL | 788 | 1652 | 63 |
| CPPCTHAL | 460 | 1324 | 46 |
| CubeMX HAL | 1668 | 2660 | 189 |
| CubeMX LL | 976 | 1904 | 50 |

Table 1: MultiCall comparison from [3], slide 27. Macro and NoHAL denote the direct-access variant in the two charts.

In this comparison, CPPCTHAL requires 460 bytes for `main` and 1324 bytes in total. Total flash usage is approximately 20% lower than CPPHAL and approximately 50% lower than CubeMX HAL. With 46 instructions, CPPCTHAL achieves the same instruction count in the loop as direct access; CPPHAL requires 63 instructions.

Fewer instructions may also reduce execution time. Their number alone, however, does not determine an exact duration: instruction types, memory accesses, and controller characteristics also contribute. The table does not provide a timing measurement. The results apply to this build and scenario; compiler and optimization settings need to be considered when making comparisons on other hardware.

## Newer language features simplify the implementation

CPPCTHAL23 applies the same basic idea using newer language features [1, 2]. Fold expressions combine multiple configurations more concisely. `if constexpr` selects a code branch at compile time. `consteval` requires evaluation at compile time; concepts express requirements for valid configurations. These features can make the implementation and error messages easier to understand. The comparison above concerns CPPCTHAL based on C++11 language features.

## Where the approach fits

A Compile-Time HAL suits fixed wiring and known operating modes. Changing a mapping requires a new build. A dynamic interface remains useful for pins that need to be selected or reconfigured at runtime.

The calculations can increase compilation time. Benefits and costs should be evaluated for each project.

Configuration checks prevent certain errors but do not guarantee the correctness of all hardware accesses. Clock enablement, initialization order, and register characteristics still need to be considered. If multiple parts of a program modify the same registers, their accesses must be coordinated.

## Conclusion and opportunities for chip manufacturers

CPPCTHAL combines simple device use with fixed connections, precomputed register values, and early checks. MultiCall shows less code and fewer instructions than several of the HAL variants examined. Execution time may also benefit, but this needs to be measured for the particular build. For a complete HAL implementation, the extent to which different specializations increase code size also needs to be investigated.

A complete C++ HAL from a chip manufacturer could extend the approach beyond GPIO to timers, ADCs, and communication interfaces. Fixed peripheral instances and operating modes could be described through template parameters and checked for valid combinations at compile time. The manufacturer could supply the corresponding register accesses and initialization sequences for the entire controller. Developers would then no longer need to build this foundation themselves for each controller. A clear interface, useful error messages, and understandable examples would be essential. Dynamic configuration would remain available where parameters are only known at runtime.

## References and further source code

[1] Michael Trummer: Compile-Time-HAL. CPPCTHAL and CPPCTHAL23. [https://github.com/michitrummer/Compile-Time-HAL](https://github.com/michitrummer/Compile-Time-HAL) — revision used: commit d72ac3d; accessed October 8, 2026.

[2] C++ working draft: Fold expressions, `constexpr`, and `consteval`. [https://eel.is/c++draft/expr.prim.fold](https://eel.is/c++draft/expr.prim.fold) and [https://eel.is/c++draft/dcl.constexpr](https://eel.is/c++draft/dcl.constexpr)

[3] Michael Trummer: Compile Time HAL. Presentation slides, revision dated October 6, 2026, particularly slides 26–31.

## Short biography

Michael Trummer studied at HSR University of Applied Sciences Rapperswil from 2009 to 2012. His professional background is in software development and project work. He currently works in Business Development at IMT Information Management Technology AG, connecting customer requirements with technical questions about architecture, project planning, and quality assurance. He is also a lecturer in the Bachelor's program in Electrical and Computer Engineering at OST – Eastern Switzerland University of Applied Sciences. The Compile-Time HAL presented in this paper is based on his own concept and implementation.
