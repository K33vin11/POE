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

// Dummy-Sensorarray (nicht benötigt für Rotation)
EdgeSensor* sensors[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

// Gyro
Gyro gyro;

// DriveSystem
DriveSystem driveSystem(&wheel1, &wheel2, &wheel3, &wheel4, sensors, &gyro);

void setUp() {}
void tearDown() {}

void test_rotate_180_degrees() {
    Serial.println("--- TEST: 180° Rotation ---");

    driveSystem.begin();
    delay(1000); // Zeit für Gyro-Initialisierung

    float startAngle = gyro.getOrientation();
    float targetAngle = startAngle + 180.0;
    if (targetAngle >= 360.0) targetAngle -= 360.0;

    Serial.print("Startwinkel: ");
    Serial.println(startAngle);
    Serial.print("Zielwinkel: ");
    Serial.println(targetAngle);

    driveSystem.move(0.0, 0.0, 0.4); // Rotation starten (omega > 0 = rechtsdrehung)

    unsigned long startTime = millis();
    bool timedOut = false;

    while (true) {
        Serial.println(gyro.getOrientation());
        float current = gyro.getOrientation();
        float diff = fabs(current - targetAngle);
        if (diff > 180.0) diff = 360.0 - diff;

        if (diff < 5.0) {
            driveSystem.stop(); // <--- Sofort stoppen
            break;
        }

        if (millis() - startTime > 5000) {
            driveSystem.stop(); // <--- Sicherheitshalber auch hier
            TEST_FAIL_MESSAGE("Rotation timed out (mehr als 10 Sekunden)");
            return;
        }

        delay(20);
    }


    driveSystem.stop();
    float finalAngle = gyro.getOrientation();

    Serial.print("Endwinkel: ");
    Serial.println(finalAngle);

    if (timedOut) {
        TEST_FAIL_MESSAGE("Rotation timed out (mehr als 10 Sekunden)");
        return;
    }

    float error = fabs(finalAngle - targetAngle);
    if (error > 180.0) error = 360.0 - error;

    char message[64];
    snprintf(message, sizeof(message), "Abweichung %.2f° – sollte < 10° sein", error);
    TEST_ASSERT_LESS_THAN_FLOAT_MESSAGE(10.0, error, message);
}

void setup() {
    Serial.begin(9600);
    Wire.begin();
    delay(2000); // Serielle Verbindung und Sensorstart
    UNITY_BEGIN();
    RUN_TEST(test_rotate_180_degrees);
    UNITY_END();
}

void loop() {
    // nichts
}
