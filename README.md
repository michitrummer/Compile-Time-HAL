# Compile-Time HAL

Dieses Repository zeigt, wie sich feste Hardwarezuordnungen und GPIO-Konfigurationen in C++ beim Übersetzen auflösen lassen. Im Mittelpunkt steht **CPPCTHAL**: Port, Pin und Betriebsart werden über Templateparameter und Traits beschrieben. Der Compiler kann daraus Registeradressen, Masken und Werte ableiten und die Zugriffsschichten bis zum Hardwarezugriff optimieren.

Die Idee und Umsetzung der Compile-Time HAL stammen von **Michael Trummer**. Das Repository enthält außerdem eine C++23-Variante sowie weitere HAL-Implementierungen für den Vergleich auf dem STM32L433.

Der [Tagungsbandbeitrag zum ESE Kongress 2026](docs/ESE_2026_Tagungsband_Compile-Time-HAL.md) erklärt den Ansatz anhand von LED- und Tasterzugriffen, vorberechneten Registerwerten und der gemeinsamen Konfiguration mehrerer Pins eines Ports.

## Beispiel mit CPPCTHAL

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

`LedTraits` ordnet `bsl::LedId::ld4` dem Ausgang PB13 zu. Die Anwendung verwendet den logischen Gerätebezeichner; Port und Pin stehen im konkreten Typ fest. `BoardConfig::apply()` aktiviert die GPIO-Takte und übernimmt die vorberechnete Konfiguration in die Register. Die Registerzugriffe selbst finden zur Laufzeit statt.

Der Kern des Ansatzes besteht aus drei Schritten:

- **Gerätezuordnung:** Traits verbinden logische Geräte mit festen Pins und deren Konfiguration.
- **Konfiguration:** Templates berechnen die benötigten Registermasken und Werte beim Übersetzen.
- **Aggregation:** Die Konfigurationen mehrerer Pins eines Ports werden zusammengeführt; Portzuordnung und doppelte Pinbelegungen werden dabei geprüft.

Feste Zuordnungen erleichtern dem Compiler das Optimieren der Zugriffe. Das Ergebnis hängt weiterhin von Optimierungsoptionen und der Sichtbarkeit der Implementierung ab. Ändert sich die Hardwarezuordnung, ist ein neuer Build erforderlich.

## Implementierungen

| Verzeichnis | Ansatz |
| --- | --- |
| [CPPCTHAL](CPPCTHAL/) | Compile-Time HAL mit Templates, Traits und vorberechneter GPIO-Konfiguration |
| [CPPCTHAL23](CPPCTHAL23/) | Modernisierte Variante mit C++23 und weiteren Sprachmitteln aus C++17 und C++20 |
| [CHAL](CHAL/) | C-HAL als Vergleich |
| [CPPHAL](CPPHAL/) | Klassische C++-HAL als Vergleich |
| [CubeMXHAL](CubeMXHAL/) | STM32Cube HAL |
| [CubeMXLL](CubeMXLL/) | STM32Cube LL |
| [NoHAL](NoHAL/) | Direkte Registerzugriffe |

CPPCTHAL setzt den Ansatz mit Sprachmitteln aus C++11 um; die aktuellen CMake-Targets verwenden C++17. CPPCTHAL23 verwendet unter anderem Fold Expressions, `if constexpr`, `consteval`, Concepts und `std::to_underlying`. Details stehen in der [Beschreibung von CPPCTHAL23](CPPCTHAL23/README_CTHAL23.md).

## Build

Für die vorhandenen ARM-GCC-Presets werden benötigt:

- CMake **3.24 oder neuer**, da die Presets das [Schema in Version 5](https://cmake.org/cmake/help/v3.24/manual/cmake-presets.7.html#format) verwenden.
- Ninja.
- GNU Arm Embedded Toolchain mit `arm-none-eabi-gcc`, `arm-none-eabi-g++` und den zugehörigen Binutils im `PATH`.
- Newlib und die ARM-C++-Standardbibliothek. Für CPPCTHAL23 müssen Compiler und Standardbibliothek die verwendeten C++23-Funktionen einschließlich `std::to_underlying` unterstützen.

Das Repository enthält einen [Dev Container](.devcontainer/devcontainer.json) für VS Code mit der ARM-Toolchain. Dazu sind Docker und die VS-Code-Erweiterung Dev Containers erforderlich.

Nach dem Klonen im Repository-Verzeichnis konfigurieren und bauen:

```sh
git clone https://github.com/michitrummer/Compile-Time-HAL.git
cd Compile-Time-HAL

cmake --preset arm-gcc-release
cmake --build --preset arm-gcc-release --parallel
```

Für einen Debug-Build in beiden Befehlen `arm-gcc-debug` verwenden. Der vollständige Build umfasst sieben Firmware-Targets und 14 Benchmark-Targets für die beiden Beispielszenarien.

Nach der Konfiguration lässt sich auch nur CPPCTHAL bauen:

```sh
cmake --build --preset arm-gcc-release --target CPPCTHAL
```

Die Build-Ausgaben liegen unter `build/arm-gcc-release/` beziehungsweise `build/arm-gcc-debug/`. Zusätzlich zu den ELF-Dateien entstehen bei verfügbarer `arm-none-eabi-objcopy` auch HEX- und BIN-Dateien. Die Zielhardware ist der STM32L433 mit Cortex-M4.

## Vergleichsbeispiele und Auswertung

Die [Beispiele](Examples/README.md) verwenden dieselbe grundlegende Aufgabe: einen Taster lesen und den zugehörigen LED-Ausgang steuern.

- [Single](Examples/Single/): ein B1/LD4-Paar.
- [MultiCall](Examples/MultiCall/README.md): das B1/LD4-Paar und acht zusätzliche Beispielpaare mit verschiedenen Pins und Konfigurationen.

Die Bezeichner `ex1` bis `ex8` sind Zuordnungen für den Benchmark und stehen für zusätzliche Beispielanschlüsse.

Die Auswertung benötigt zusätzlich Bash und Python 3:

```sh
bash MemoryAnalysis.sh
```

[MemoryAnalysis.sh](MemoryAnalysis.sh) erstellt isolierte Benchmark-ELFs und vollständige Firmware-ELFs. [MemoryAnalysisResults.py](MemoryAnalysisResults.py) wertet Speicherbedarf und Instruktionsanzahl aus; die Ergebnisse liegen unter `MemoryAnalysis/`.

Die Ergebnisse hängen von Toolchain, Optimierung und Szenario ab. Eine Instruktionsanzahl ist keine Laufzeitmessung. Der Tagungsbandbeitrag ordnet die dort gezeigten Vergleichswerte für den verwendeten Stand ein.

## Lizenz

Projektdateien mit `SPDX-License-Identifier: Apache-2.0` stehen unter der [Apache License 2.0](LICENSE). Für mitgelieferte oder generierte Dateien von STMicroelectronics gelten deren jeweilige Lizenzhinweise. Einzelheiten stehen in [LICENSE_SCOPE.md](LICENSE_SCOPE.md) und [NOTICE](NOTICE).
