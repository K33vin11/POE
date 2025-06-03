#include <Arduino.h>
#include <unity.h>
#include <Config.h>
#include <Wheel.h>
#include <Motor.h>
#include <Encoder.h>
#include <Gyro.h>
#include <DriveSystem.h>

// Wheels
Wheel wheel1(MOTOR1_I2C_ADDRESS, ENCODER1_A_PIN, ENCODER1_B_PIN, WHEEL_DIAMETER / 100.0, MOTOR1_DIRECTION);
Wheel wheel2(MOTOR2_I2C_ADDRESS, ENCODER2_A_PIN, ENCODER2_B_PIN, WHEEL_DIAMETER / 100.0, MOTOR2_DIRECTION);
Wheel wheel3(MOTOR3_I2C_ADDRESS, ENCODER3_A_PIN, ENCODER3_B_PIN, WHEEL_DIAMETER / 100.0, MOTOR3_DIRECTION);
Wheel wheel4(MOTOR4_I2C_ADDRESS, ENCODER4_A_PIN, ENCODER4_B_PIN, WHEEL_DIAMETER / 100.0, MOTOR4_DIRECTION);

// Dummy-Sensorarray (nicht benötigt für Fahrt)
EdgeSensor* sensors[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

// Gyro
Gyro gyro;

// DriveSystem
DriveSystem driveSystem(&wheel1, &wheel2, &wheel3, &wheel4, sensors, &gyro);

float startwinkel;
float abweichung;
bool first = false; // Flag für erste Fahrt

float angleDifference(float target, float current) {
    float diff = fmod((current - target + 540.0), 360.0) - 180.0;
    return diff;
}



void setUp() {}
void tearDown() {}

void test_drive_forward_10_seconds() {

    Serial.println("--- TEST: 10 Sekunden Vorwärtsfahrt ---");

    driveSystem.begin();
        driveSystem.setSpeedLimit(0.55); // Maximal 60 % Speed
    delay(1000); // Initialisierung

    startwinkel = gyro.getOrientation(); // saubere Initialisierung vor Schleife

    unsigned long startTime = millis();
    while (millis() - startTime < 30000) {
    }

    driveSystem.stop();

    TEST_PASS_MESSAGE("Fahrt für 10 Sekunden erfolgreich beendet");
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(2000); // Serielle Verbindung und Sensorstart
    UNITY_BEGIN();
    RUN_TEST(test_drive_forward_10_seconds);
    UNITY_END();
}

void loop() {
    // nichts
}
