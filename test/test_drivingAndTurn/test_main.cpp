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

void test_driving() {
    Serial.println("--- TEST: 20 cm Forward ---");
    driveSystem.begin();
    driveSystem.setSpeedLimit(0.6); // Maximal 60 % Speed
    delay(1000); // Gyro stabilisieren

    // Nur 20 cm vorwärts fahren
    if (driveSystem.goTo(1, 0.0)) {
        delay(10);
    

    // Anhalten und Position ausgeben
    driveSystem.stop();
    Location loc = driveSystem.getCurrentLocation();
    Serial.print("X: "); Serial.println(loc.x);
    Serial.print("Y: "); Serial.println(loc.y);
    Serial.print("Theta: "); Serial.println(loc.theta);

    // Testbedingungen (Toleranz 1 cm, 5 Grad)
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.2, loc.x);
    TEST_ASSERT_FLOAT_WITHIN(0.01, 0.0, loc.y);
    TEST_ASSERT_FLOAT_WITHIN(5.0, 0.0, loc.theta);
    }
}



void setup() {
    Serial.begin(9600);
    delay(2000); // Serielle Verbindung und Sensorstart
    UNITY_BEGIN();
    RUN_TEST(test_driving);
    UNITY_END();
}

void loop() {
    driveSystem.updatePosition(); // Position aktualisieren
}
