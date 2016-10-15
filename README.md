# Cyber
Ein JugendHackt-Berlin Projekt zum Einbruchsschutz.

## Hardware
- Arduino Nano
- Funkempfänger und -sender
- SMS-Sender inklusive SIM-Karte
- Raspberry PI 3 Model B
- Bewegungsmelder und andere Sensoren

## Funktionsweise
Ein externes Modul, welches von einem Arduino gesteuert wird, ließt Daten der Sensoren ein und sendet diese über Funk an ein Hauptmodul bestehend aus einem Raspberry PI. Insofern Schwellwerte erreicht werden, wird vermutet, dass eingebrochen wird. In diesem Fall wird eine SMS an den Benutzer gesendet, um ihn über den Einbruch zu informieren.
