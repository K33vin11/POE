#include <Encoder.h>
#include <Arduino.h>

// Statische Instanzliste für bis zu 4 Encoder
// Damit die Interrupt-Service-Routinen (ISR) wissen, welche Instanz sie aufrufen
static Encoder* instances[4] = { nullptr };

// ISR-Verteilerfunktionen für die 4 möglichen Encoder-Interrupts
// Jede ISR ruft die zugehörige Instanzmethode handleInterrupt() auf
static void isr0() { if (instances[0]) instances[0]->handleInterrupt(); }
static void isr1() { if (instances[1]) instances[1]->handleInterrupt(); }
static void isr2() { if (instances[2]) instances[2]->handleInterrupt(); }
static void isr3() { if (instances[3]) instances[3]->handleInterrupt(); }

// Konstruktor: Pins, Pulse und Index initialisieren, Richtung invertierbar
Encoder::Encoder(int a, int b, bool invert) 
    : pinA(a), pinB(b), pulses(0), encoderIndex(-1), invert(invert) {}

// Initialisierung: Pins als Eingang mit Pullup, Interrupts registrieren
void Encoder::begin() {
    pinMode(pinA, INPUT_PULLUP); // Pin A als Eingang mit Pullup-Widerstand
    pinMode(pinB, INPUT_PULLUP); // Pin B als Eingang mit Pullup-Widerstand

    // Suche eine freie Stelle im Instanz-Array für Interrupt-Zuweisung
    for (int i = 0; i < 4; i++) {
        if (instances[i] == nullptr) {
            instances[i] = this;  // Diese Instanz dort speichern
            encoderIndex = i;     // Eigener Index merken

            // Interrupt für pinA an die passende ISR anhängen (Rising Edge)
            switch (i) {
                case 0: attachInterrupt(digitalPinToInterrupt(pinA), isr0, RISING); break;
                case 1: attachInterrupt(digitalPinToInterrupt(pinA), isr1, RISING); break;
                case 2: attachInterrupt(digitalPinToInterrupt(pinA), isr2, RISING); break;
                case 3: attachInterrupt(digitalPinToInterrupt(pinA), isr3, RISING); break;
            }
            break;
        }
    }
}

// Wird von der ISR aufgerufen, bei steigendem Signal an pinA
// Liest Zustand von pinB, um Drehrichtung zu bestimmen und Pulse zu zählen
void Encoder::handleInterrupt() {
    bool pinBState = digitalRead(pinB);
    if (!invert) {
        if (pinBState) {
            pulses++; // Drehrichtung: vorwärts
        } else {
            pulses--; // Drehrichtung: rückwärts
        }
    } else {
        // Invertierte Drehrichtung
        if (pinBState) {
            pulses--; 
        } else {
            pulses++;
        }
    }
}

// Gibt aktuellen Pulsstand zurück (atomar, Interrupts kurz deaktiviert)
long Encoder::readPulses() {
    noInterrupts();    // Interrupts sperren für konsistente Daten
    long temp = pulses;
    interrupts();      // Interrupts wieder freigeben
    return temp;
}

// Setzt Pulszähler zurück (atomar)
void Encoder::reset() {
    noInterrupts();
    pulses = 0;
    interrupts();
}

// Berechnet die Distanz basierend auf Pulsanzahl
// Hier angenommen 1 Puls = 1 mm, daher Multiplikation mit 0.001 (Meter)
float Encoder::calculateDistance() {
    return readPulses() * 0.001;
}
