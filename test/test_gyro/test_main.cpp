#include <Arduino.h>
#include <unity.h>
#include <Gyro.h>

Gyro gyro;

void setUp(void) {
    // Vor jedem Test (z. B. Initialisierung)
}

void tearDown(void) {
    // Nach jedem Test
}

void test_gyro_returns_valid_orientation() {
    gyro.begin();
    delay(500); // Sensor initialisieren lassen

    unsigned long startTime = millis();

    while (millis() - startTime <= 20000) {
        float orientation = gyro.getOrientation();

        Serial.print("Gyro orientation: ");
        Serial.println(orientation);

        delay(20);
    }

    // Letzter Wert zur Prüfung
    float finalOrientation = gyro.getOrientation();

    // Erwartet Werte im Bereich 0 bis <360
    TEST_ASSERT_GREATER_OR_EQUAL_FLOAT(0.0, finalOrientation);
    TEST_ASSERT_LESS_THAN_FLOAT(360.0, finalOrientation);
}

void setup() {
    delay(2000); // Warten bis Serial Monitor bereit
    Serial.begin(9600);

    UNITY_BEGIN();
    RUN_TEST(test_gyro_returns_valid_orientation);
    UNITY_END();
}

void loop() {
    // bleibt leer
}
