# Compile Time HAL

**Feste Hardwarekonfiguration mit C++ zur Übersetzungszeit auflösen**

Michael Trummer, IMT Information Management Technology AG

**Eine Compile Time HAL verankert feste Hardwarezuordnungen in C++-Typen und berechnet Konfigurationswerte beim Übersetzen. An der C++11-Implementierung CPPCTHAL zeigt der Beitrag, wie Templateparameter und Traits die Gerätezuordnung festlegen, Registerwerte erzeugen und mehrere Pinbeschreibungen zusammenführen. Ein Vergleich betrachtet Codeumfang und Ausführungsaufwand; CPPCTHAL23 ergänzt den Ausblick.**

## Feste Anschlüsse als Ausgangspunkt

Für eine fest verdrahtete LED sind Portadresse und Bitmaske bekannt. Viele HAL-Schnittstellen ermitteln sie dennoch bei jedem Zugriff aus einer Gerätenummer, etwa über eine Tabelle oder unterschiedliche Codezweige. Das verursacht zusätzliche Speicherzugriffe oder Verzweigungen für eine unveränderliche Zuordnung. In Handles gespeicherte Hardwareparameter belegen zudem RAM. CPPCTHAL legt die Zuordnung im Typ fest, sodass der Compiler den Zugriff auf ein konstantes Registerziel zurückführen kann.

CPPCTHAL beschreibt `Port`, `Pin` und Betriebsart mit Templateparametern. Die gewählte Hardware wird damit Teil des konkreten C++-Typs. Für diesen Typ kann der Compiler Registeradressen, Bitmasken und Konfigurationswerte ableiten. Der Hardwarezugriff bleibt eine Laufzeitoperation; Geräteauswahl und Berechnung fester Werte finden beim Übersetzen statt.

CPPCTHAL [1] setzt den Ansatz für den STM32L433 mit C++11 um. Der vollständige Quellcode einschließlich der Vergleichsprojekte ist im folgenden Repository verfügbar:

[https://github.com/michitrummer/Compile-Time-HAL](https://github.com/michitrummer/Compile-Time-HAL)

## LED und Taster über Templateparameter auswählen

`LD4` und B1 sind im Beispiel die logischen Gerätebezeichner für die LED an PB13 und den Taster an PC13. `Traits` ordnen ihnen den konkreten `Pin`-Typ und dessen Konfiguration zu: PB13 als Ausgang, PC13 als Eingang. Bei einer anderen Platine wird diese zentrale Zuordnung angepasst; die Anwendungslogik kann ihre Gerätenamen beibehalten.

Listing 1 zeigt die Anwendungslogik in `main`. `bsl` bezeichnet die Board-Schicht. `Led<bsl::LedId::ld4>` und `Switch<bsl::SwitchId::b1>` instanziieren Gerätetypen für die fest ausgewählten Geräte. Die Auswahl steht im Typ und wird beim Übersetzen aufgelöst. Die Aufrufe `on()`, `off()` und `pressed()` benötigen dadurch keine Verzweigungslogik zur Geräteauswahl.

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

Listing 1: Anwendung mit fester LED- und Taster-Zuordnung. Quelle: vereinfacht aus [1], `CPPCTHAL/Src/main.cpp`.

`BoardConfig::apply()` aktiviert die GPIO-Takte und konfiguriert die Anschlüsse. Die Schleife in `main` verarbeitet anschließend den Tasterpegel und steuert die LED. Dieser Teil der Anwendungslogik verwendet die logischen Geräte; konkrete Registeradressen und Bitmasken sind in der HAL hinterlegt und beim Übersetzen bekannt. Im erzeugten Programm werden sie für die Hardwarezugriffe verwendet.

## Registerwerte aus Templateparametern ableiten

Der nächste Schritt ist die Ableitung der Registerwerte aus dem `Pin`-Typ. In CPPCTHAL übernimmt eine Konfigurationsklasse diese Berechnung anhand von Templateparametern für Pinnummer, Betriebsart und weitere Einstellungen. Listing 2 reduziert diese Klasse auf das Modusfeld in `MODER`; die Berechnung entspricht dem Prinzip aus CPPCTHAL.

Für die LED sind `Pin` 13 und Ausgangsmodus bereits beim Übersetzen vorgegeben. Die Konfigurationsklasse liefert daraus die `MODER`-Maske `0x0C000000` und den Wert `0x04000000`. Es ist keine Laufzeitfunktion nötig, um diese Werte für den konkreten `Pin` zu berechnen.

`ModeConfig` bildet den Modusanteil einer Pinbeschreibung ab. `LedMode` ist die Spezialisierung für `Pin` 13 als Push-Pull-Ausgang. Die kommentierten Beispielwerte gelten für diese Spezialisierung.

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
// Beispielwerte dieser Spezialisierung:
// mask = 0x0C000000u, value = 0x04000000u
```

Listing 2: Registerkonstanten aus Pinnummer und Betriebsart. Quelle: auf `MODER` reduzierte Darstellung aus [1], CPPCTHAL/`csl`/include/`GpioConfig.h`.

Die `enum`-Konstanten entstehen bei der Instanziierung von `ModeConfig` aus den Templateargumenten `Id` und `Mode`. Sie bilden keinen veränderlichen Konfigurationszustand. CPPCTHAL verwendet dieses Muster auch für Ausgangstyp, Geschwindigkeit und Pull-Widerstände.

Die Initialisierung übernimmt anschließend die vorberechneten Masken und Werte und schreibt sie zur Laufzeit in die Register. Compile-Time-Konfiguration bedeutet daher nicht, dass Hardware beim Übersetzen eingerichtet wird: Der Compiler erzeugt die Konstanten; der ausgeführte Initialisierungscode verwendet sie.

## Mehrere Pins gemeinsam vorbereiten

Listing 3 ergänzt die LED `LD4` an PB13 um `EX1`, einen zusätzlichen Beispielausgang an PB0. Die zugehörigen `LedTraits` legen `EX1` als Push-Pull-Ausgang fest. Beide Pins gehören zu `Port` B und lassen sich gemeinsam konfigurieren.

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

Listing 3: Zwei Pins gemeinsam für `Port` B konfigurieren. Quelle: vereinfacht nach [1], CPPCTHAL/`bsl`/include/BoardConfig.h und `Led.h`.

`PortConfiguration` erhält den `Port` und die `Traits`-Typen als Templateargumente. Das variadische Template nimmt mehrere Pinbeschreibungen entgegen. CPPCTHAL führt deren Masken und Werte rekursiv beim Übersetzen zusammen. `configurePortB()` aktiviert zuerst den gemeinsamen GPIOB-Takt und schreibt anschließend über `PortB::apply()` die vorberechnete Konfiguration in die Register.

CPPCTHAL prüft bei der Übersetzung die Portzuordnung und weist doppelte Konfigurationen desselben Pins zurück. Für die LED an PB13 und den Taster an PC13 bleiben getrennte Portkonfigurationen erforderlich; `BoardConfig::apply()` aus Listing 1 übernimmt sie.

## Warum die Zugriffsschichten verschwinden können

`on()` führt über den durch die `Traits` festgelegten `Pin`-Typ zum Hardwarezugriff. `Port` und `Pin` stehen in jeder beteiligten Spezialisierung fest. Kann der Compiler die in den Headern definierten kleinen Zugriffsfunktionen inline einsetzen, lassen sich die Schichten bis zum Registerzugriff mit konstantem Ziel und Schreibwert zusammenführen.

Das hängt von Optimierung und Sichtbarkeit des Codes ab. Auch klassische C-Schnittstellen können mit Konstanten effizient übersetzt werden. CPPCTHAL macht feste Hardwareentscheidungen ausdrücklich in der Schnittstelle sichtbar und prüfbar.

## Codeumfang und Ausführungsaufwand im Vergleich

Im MultiCall-Szenario werden mehrere LED- und Tasterzugriffe mit verschiedenen HAL-Varianten ausgeführt [3]. Tabelle 1 vergleicht Flash-Bedarf und Instruktionen in der Hauptschleife. Flash `main` bezeichnet den Codeumfang der Funktion `main`; Flash gesamt umfasst das vollständige Programm.

| Variante | Flash main [Byte] | Flash gesamt [Byte] | Instruktionen Hauptschleife |
| --- | ---: | ---: | ---: |
| Makros / NoHAL | 1284 | 2148 | 46 |
| CHAL | 780 | 1644 | 63 |
| CPPHAL | 788 | 1652 | 63 |
| CPPCTHAL | 460 | 1324 | 46 |
| CubeMX HAL | 1668 | 2660 | 189 |
| CubeMX LL | 976 | 1904 | 50 |

Tabelle 1: MultiCall-Vergleich aus [3], Folie 27. Macro und NoHAL bezeichnen in den beiden Diagrammen die direkte Zugriffsvariante.

CPPCTHAL benötigt in diesem Vergleich 460 Byte für `main` und 1324 Byte insgesamt. Gegenüber CPPHAL sinkt der gesamte Flash-Bedarf um rund 20 %, gegenüber CubeMX HAL um rund 50 %. Mit 46 Instruktionen erreicht CPPCTHAL denselben Schleifenwert wie direkte Zugriffe; CPPHAL benötigt 63 Instruktionen.

Weniger Instruktionen können auch die Laufzeit verkürzen. Aus ihrer Anzahl allein folgt jedoch keine genaue Zeit: Instruktionstypen, Speicherzugriffe und Controller-Eigenschaften wirken mit. Die Tabelle liefert keine Zeitmessung. Die Ergebnisse gelten für diesen Build und dieses Szenario; Compiler und Optimierung sind beim Vergleich auf eigener Hardware zu berücksichtigen.

## Neuere Sprachmittel erleichtern die Umsetzung

CPPCTHAL23 nutzt dieselbe Grundidee mit neueren Sprachmitteln [1, 2]. Fold Expressions verknüpfen mehrere Konfigurationen kompakter. `if constexpr` wählt einen Codezweig beim Übersetzen aus. `consteval` verlangt Berechnungen zur Übersetzungszeit; Concepts beschreiben Anforderungen an zulässige Konfigurationen. Dies kann Implementierung und Fehlermeldungen verständlicher machen. Der Vergleich betrifft CPPCTHAL mit C++11.

## Wo der Ansatz passt

Eine Compile Time HAL passt zu fester Verdrahtung und bekannten Betriebsarten. Ändert sich die Zuordnung, ist ein neuer Build erforderlich. Für Anschlüsse, die zur Laufzeit ausgewählt oder umkonfiguriert werden müssen, bleibt eine dynamische Schnittstelle sinnvoll.

Die Berechnung kann die Übersetzungszeit erhöhen. Nutzen und Aufwand sind am eigenen Projekt zu prüfen.

Konfigurationsprüfungen verhindern bestimmte Fehler, garantieren aber nicht die Korrektheit aller Hardwarezugriffe. Taktfreigabe, Initialisierungsreihenfolge und Registereigenschaften bleiben zu beachten. Verändern mehrere Programmteile dieselben Register, müssen ihre Zugriffe aufeinander abgestimmt werden.

## Fazit und Perspektive für Chiphersteller

CPPCTHAL verbindet einfache Gerätenutzung mit festen Anschlüssen, vorberechneten Registerwerten und frühen Prüfungen. MultiCall zeigt weniger Code und Instruktionen als mehrere der untersuchten HAL-Varianten. Auch die Laufzeit kann profitieren; dies ist im jeweiligen Build zu messen. Für eine vollständige HAL-Implementierung ist zudem zu untersuchen, wie stark unterschiedliche Spezialisierungen den Codeumfang vergrößern.

Eine vollständige C++-HAL vom Chiphersteller könnte den Ansatz über GPIO hinaus auf Timer, ADCs und Kommunikationsschnittstellen übertragen. Feste Peripherieinstanzen und Betriebsarten ließen sich über Templateparameter beschreiben und beim Übersetzen auf zulässige Kombinationen prüfen. Der Hersteller könnte dazu die passenden Registerzugriffe und Initialisierungsfolgen für den gesamten Controller bereitstellen. Entwickler müssten diese Grundlage dann nicht für jeden Controller selbst aufbauen. Entscheidend wären eine verständliche Schnittstelle, brauchbare Fehlermeldungen und nachvollziehbare Beispiele. Dynamische Konfiguration bliebe dort verfügbar, wo Parameter erst zur Laufzeit feststehen.

## Quellen und weiterführender Code

[1] Michael Trummer: Compile-Time-HAL. CPPCTHAL und CPPCTHAL23. [https://github.com/michitrummer/Compile-Time-HAL](https://github.com/michitrummer/Compile-Time-HAL) — verwendeter Stand: Commit d72ac3d; Abruf 8.10.2026.

[2] C++-Arbeitsentwurf: Fold expressions, `constexpr` und `consteval`. [https://eel.is/c++draft/expr.prim.fold](https://eel.is/c++draft/expr.prim.fold) und [https://eel.is/c++draft/dcl.constexpr](https://eel.is/c++draft/dcl.constexpr)

[3] Michael Trummer: Compile Time HAL. Vortragsfolien, Stand 6.10.2026, insbesondere Folien 26–31.

## Kurzbiografie

Michael Trummer studierte von 2009 bis 2012 an der HSR Hochschule für Technik Rapperswil. Sein beruflicher Hintergrund liegt in der Softwareentwicklung und Projektarbeit. Heute ist er im Business Development der IMT Information Management Technology AG tätig. Dort verbindet er Kundenanforderungen mit technischen Fragen zu Architektur, Projektplanung und Qualitätssicherung. Daneben ist er Dozent im Bachelorstudiengang Electrical and Computer Engineering an der OST – Ostschweizer Fachhochschule. Die im Beitrag vorgestellte Compile Time HAL basiert auf seiner eigenen Idee und Umsetzung.
