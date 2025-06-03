#include <BridgePlacer.h>
#include <Arduino.h>
#include <Config.h>

// Konstruktor: Initialisiert Servo- und Richtungspin sowie die Richtung
BridgePlacer::BridgePlacer(int motorPin, int directionPin, bool direction)
  : servoPin(motorPin), dirPin(directionPin), dir(direction) {}

// Initialisierung der Pins als Ausgang
void BridgePlacer::begin() {
    pinMode(servoPin, OUTPUT); // Servo-Pin als Ausgang setzen
    pinMode(dirPin, OUTPUT);   // Richtungspin als Ausgang setzen
}

// Brücke absenken für eine bestimmte Zeit (Millisekunden)
void BridgePlacer::Down(int time) {
    digitalWrite(dirPin, dir ? HIGH : LOW); // Richtung setzen
    digitalWrite(servoPin, HIGH);            // Servo aktivieren (Motor starten)
    delay(time);                             // Wartezeit abwarten
    digitalWrite(servoPin, LOW);             // Servo stoppen (Motor ausschalten)
    Serial.println("Brücke abgelegt");       // Debug-Ausgabe
}

// Brücke anheben für eine bestimmte Zeit (Millisekunden)
void BridgePlacer::Up(int time) {
    digitalWrite(dirPin, dir ? LOW : HIGH); // Richtung umkehren
    digitalWrite(servoPin, HIGH);           // Servo aktivieren (Motor starten)
    delay(time);                            // Wartezeit abwarten
    digitalWrite(servoPin, LOW);            // Servo stoppen (Motor ausschalten)
    Serial.println("Brücke angehoben");     // Debug-Ausgabe
}
