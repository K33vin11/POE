#include <Arduino.h>
#include <unity.h>
#include <Motor.h>
#include <Encoder.h>
#include <Config.h>
#include <Wire.h>

// --- Globale Objekte ---
Motor motors[4] = {
  Motor(MOTOR1_I2C_ADDRESS, false),
  Motor(MOTOR2_I2C_ADDRESS, true),
  Motor(MOTOR3_I2C_ADDRESS, false),
  Motor(MOTOR4_I2C_ADDRESS, true)
};

Encoder encoders[4] = {
  Encoder(ENCODER1_A_PIN, ENCODER1_B_PIN, false),
  Encoder(ENCODER2_A_PIN, ENCODER2_B_PIN, true),
  Encoder(ENCODER3_A_PIN, ENCODER3_B_PIN, false),
  Encoder(ENCODER4_A_PIN, ENCODER4_B_PIN, true)
};

// --- Setup vor jedem Test ---
void setUp() {
  for (int i = 0; i < 4; i++) {
    encoders[i].reset();
    motors[i].stop();
  }
}

// --- Teardown nach jedem Test ---
void tearDown() {
  for (int i = 0; i < 4; i++) {
    motors[i].stop();
  }
  delay(100);
}

// --- Test: Vorwärtsfahrt – prüft nur, ob Pulse ≠ 0 ---
void test_motor_encoder_forward_any_direction() {
  Serial.println("--- TEST: Vorwärtsfahrt – Pulse ≠ 0 ---");

  for (int i = 0; i < 4; i++) {
    encoders[i].reset();
    motors[i].setSpeed(150);
  }

  delay(3000);

  for (int i = 0; i < 4; i++) {
    motors[i].stop();
  }

  delay(200);

  bool testFailed = false;

  for (int i = 0; i < 4; i++) {
    long pulses = encoders[i].readPulses();
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" Pulse: ");
    Serial.println(pulses);

    if (pulses == 0) {
      Serial.print("Fehler: Motor ");
      Serial.print(i + 1);
      Serial.println(" hat keine Pulse registriert!");
      testFailed = true;
    }
  }

  TEST_ASSERT_FALSE_MESSAGE(testFailed, "Mindestens ein Motor hat keine Pulse geliefert (vorwärts).");
}

// --- Test: Rückwärtsfahrt – ebenfalls nur Pulse ≠ 0 ---
void test_motor_encoder_reverse_any_direction() {
  Serial.println("--- TEST: Rückwärtsfahrt – Pulse ≠ 0 ---");

  for (int i = 0; i < 4; i++) {
    encoders[i].reset();
    motors[i].setSpeed(-150);
  }

  delay(3000);

  for (int i = 0; i < 4; i++) {
    motors[i].stop();
  }

  delay(200);

  bool testFailed = false;

  for (int i = 0; i < 4; i++) {
    long pulses = encoders[i].readPulses();
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" Pulse: ");
    Serial.println(pulses);

    if (pulses == 0) {
      Serial.print("Fehler: Motor ");
      Serial.print(i + 1);
      Serial.println(" hat keine Pulse registriert!");
      testFailed = true;
    }
  }

  TEST_ASSERT_FALSE_MESSAGE(testFailed, "Mindestens ein Motor hat keine Pulse geliefert (rückwärts).");
}

// --- Test: Jeder Motor einzeln vorwärts ---
void test_each_motor_forward() {
  for (int i = 0; i < 4; i++) {
    Serial.print("--- TEST: Motor ");
    Serial.print(i + 1);
    Serial.println(" einzeln vorwärts ---");

    encoders[i].reset();
    motors[i].setSpeed(150);
    delay(2000);
    motors[i].stop();
    delay(200);

    long pulses = encoders[i].readPulses();
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" Pulse: ");
    Serial.println(pulses);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, pulses, "Motor hat keine Pulse geliefert (vorwärts, einzeln).");
  }
}

// --- Test: Jeder Motor einzeln rückwärts ---
void test_each_motor_reverse() {
  for (int i = 0; i < 4; i++) {
    Serial.print("--- TEST: Motor ");
    Serial.print(i + 1);
    Serial.println(" einzeln rückwärts ---");

    encoders[i].reset();
    motors[i].setSpeed(-150);
    delay(2000);
    motors[i].stop();
    delay(200);

    long pulses = encoders[i].readPulses();
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" Pulse: ");
    Serial.println(pulses);
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, pulses, "Motor hat keine Pulse geliefert (rückwärts, einzeln).");
  }
}

// --- Test: Pulse-Zählung vorwärts und rückwärts ---
void test_motor_encoder_pulse_direction() {
  long forwardPulses[4];
  long reversePulses[4];

  // Vorwärts
  for (int i = 0; i < 4; i++) {
    encoders[i].reset();
    motors[i].setSpeed(150);
  }
  delay(2000);
  for (int i = 0; i < 4; i++) {
    motors[i].stop();
    forwardPulses[i] = encoders[i].readPulses();
    encoders[i].reset();
  }

  // Rückwärts
  for (int i = 0; i < 4; i++) {
    motors[i].setSpeed(-150);
  }
  delay(2000);
  for (int i = 0; i < 4; i++) {
    motors[i].stop();
    reversePulses[i] = encoders[i].readPulses();
  }

  for (int i = 0; i < 4; i++) {
    Serial.print("Motor ");
    Serial.print(i + 1);
    Serial.print(" Pulse vorwärts: ");
    Serial.print(forwardPulses[i]);
    Serial.print(" rückwärts: ");
    Serial.println(reversePulses[i]);

    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, forwardPulses[i], "Vorwärts keine Pulse!");
    TEST_ASSERT_NOT_EQUAL_MESSAGE(0, reversePulses[i], "Rückwärts keine Pulse!");
    TEST_ASSERT_GREATER_THAN_MESSAGE(0, forwardPulses[i], "Vorwärts Pulse sollten positiv sein!");
    TEST_ASSERT_LESS_THAN_MESSAGE(0, reversePulses[i], "Rückwärts Pulse sollten negativ sein!");
  }
}

// --- Setup ---
void setup() {
  Serial.begin(9600);
  delay(2000);
  Wire.begin();
  for (int i = 0; i < 4; i++) {
    motors[i].begin();
    encoders[i].begin();
  }

  UNITY_BEGIN();

  RUN_TEST(test_motor_encoder_forward_any_direction);
  RUN_TEST(test_motor_encoder_reverse_any_direction);
  RUN_TEST(test_each_motor_forward);
  RUN_TEST(test_each_motor_reverse);
  RUN_TEST(test_motor_encoder_pulse_direction);

  UNITY_END();
}

// --- Loop ---
void loop() {
  // leer
}
