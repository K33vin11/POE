#include <DistanceSensor.h>
#include <Arduino.h>
#include <config.h>

// Konstruktor: Initialisiert Trigger- und Echo-Pins sowie die Erkennungsschwelle (in cm)
DistanceSensor::DistanceSensor(int trig, int echo, float threshold)
    : trigPin(trig), echoPin(echo), detectionThreshold(threshold) {}

// Initialisierung des Ultraschallsensors (NewPing Objekt)
void DistanceSensor::begin() {
    // Neues NewPing-Objekt mit Trigger-, Echo-Pin und max. Reichweite erstellen
    sonar = new NewPing(trigPin, echoPin, ULTRASONIC_MAX_DISTANCE);
}

// Gibt die gemessene Entfernung in cm zurück
float DistanceSensor::getDistance() {
    if (sonar == nullptr) return -1.0; // Falls Sensor nicht initialisiert, Fehlerwert

    delay(50); // Kleine Pause, um den Sensor zu stabilisieren

    unsigned int distanceCm = sonar->ping_cm(); // Entfernung messen (cm)

    return (float)distanceCm; // Umwandlung in float und Rückgabe
}
    
// Prüft, ob ein Objekt innerhalb der definierten Distanzschwelle erkannt wurde
bool DistanceSensor::objectDetected() {
    float distance = getDistance();

    // Objekt erkannt, wenn Distanz > 0 und kleiner/gleich der Schwellwert ist
    if (distance > 0 && distance <= detectionThreshold) {
        return true;
    }

    return false; // Objekt nicht erkannt
}
