#include <Gripper.h>
#include <Arduino.h>
#include <Config.h>

// Konstruktor: Initialisiert Pins und Richtung
Gripper::Gripper(int motorPin, int directionPin, bool direction, int openPin, int closePin)
  : servoPin(motorPin), dirPin(directionPin), openP(openPin), closeP(closePin), dir(direction) {}

// Initialisierung der Pin-Modi für den Greifer
void Gripper::begin() {
    pinMode(servoPin, OUTPUT);  // Servo-Motor-Pin als Ausgang definieren
    pinMode(dirPin, OUTPUT);    // Richtungspin als Ausgang definieren
    pinMode(openP, OUTPUT);     // Öffnungspin als Ausgang definieren
    pinMode(closeP, OUTPUT);    // Schließpin als Ausgang definieren
}

// Greifer nach unten bewegen für die angegebene Zeit (Millisekunden)
void Gripper::Down(int time) {
    // Richtung setzen (HIGH oder LOW, je nach "dir")
    digitalWrite(dirPin, dir ? HIGH : LOW);
    digitalWrite(servoPin, HIGH);  // Servo aktivieren (z.B. Motor einschalten)
    delay(time);                   // Warte die angegebene Zeit ab
    digitalWrite(servoPin, LOW);   // Servo stoppen (Motor ausschalten)
    Serial.println("Brücke abgelegt"); // Debug-Ausgabe
}

// Greifer nach oben bewegen für die angegebene Zeit (Millisekunden)
void Gripper::Up(int time) {
    // Richtung invertieren (entgegengesetzt zu Down)
    digitalWrite(dirPin, dir ? LOW : HIGH);
    digitalWrite(servoPin, HIGH);  // Servo aktivieren
    delay(time);                   // Warte die angegebene Zeit ab
    digitalWrite(servoPin, LOW);   // Servo stoppen
    Serial.println("Brücke angehoben"); // Debug-Ausgabe
}

// Greifer öffnen (z.B. öffnen des Greiferarms) für angegebene Zeit
void Gripper::open(int time) {
    digitalWrite(openP, HIGH);   // Öffnungspin aktivieren
    delay(time);                 // Warte die angegebene Zeit
    digitalWrite(openP, LOW);    // Öffnungspin deaktivieren
    Serial.println("Greifer geöffnet"); // Debug-Ausgabe (kleiner Tipp: "Greifer" statt "Greiffer")
}

// Greifer schließen (z.B. schließen des Greiferarms) für angegebene Zeit
void Gripper::close(int time) {
    digitalWrite(closeP, HIGH);  // Schließpin aktivieren
    delay(time);                 // Warte die angegebene Zeit
    digitalWrite(closeP, LOW);   // Schließpin deaktivieren
    Serial.println("Greifer geschlossen"); // Debug-Ausgabe (siehe Tipp oben)
}
