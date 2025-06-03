#include <EdgeSensor.h>
#include <Arduino.h>

// Konstruktor: Pin für den Kantensensor setzen
EdgeSensor::EdgeSensor(int p) : pin(p) {}

// Initialisierung: Pin als Eingang definieren
void EdgeSensor::begin() {
    pinMode(pin, INPUT);
}

// Kanten-Erkennung: Liefert true, wenn Kante erkannt wird
// Hinweis: digitalRead(pin) == HIGH bedeutet keine Kante
//          digitalRead(pin) == LOW bedeutet Kante erkannt
bool EdgeSensor::detectEdge() {
    return digitalRead(pin) == LOW;
}
