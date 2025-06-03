#include <Gyro.h>
#include <Wire.h>
#include <Config.h>

// Konstruktor: initialisiert currentOrientation mit 0.0
Gyro::Gyro() : currentOrientation(0.0) {}

// Initialisierung des Gyrosensors
void Gyro::begin() {
    // Pin für das Gyrosignal als Eingang definieren
    pinMode(GYRO_PIN, INPUT);

    Serial.println("Gyro initialisieren...");
    Serial.println("Gyro bereit!");
}

// Liest die aktuelle Orientierung (Yaw) vom Gyro aus
float Gyro::getOrientation() {
    // Misst die Zeitspanne, in der das Signal HIGH ist
    unsigned long highTime = pulseIn(GYRO_PIN, HIGH);

    // Misst die Zeitspanne, in der das Signal LOW ist
    unsigned long lowTime  = pulseIn(GYRO_PIN, LOW);

    // Gesamtperiode des Signals (HIGH + LOW)
    unsigned long period = highTime + lowTime;

    // Falls kein Signal erkannt wird (Periode 0), gib 0 zurück
    if (period == 0) return 0;

    // Berechne Tastverhältnis (Duty Cycle) als Verhältnis HIGH zu Gesamtperiode
    float dutyCycle = (float)highTime / period;

    // Skaliere das Tastverhältnis auf 0 bis 360 Grad (entspricht Orientierung)
    return dutyCycle * 360.0;
}
