#include <Arduino.h>
#include <unity.h>
#include <NewPing.h>
#include <Config.h>

#define TRIGGER_PIN ULTRASONIC_TRIG_PIN
#define ECHO_PIN ULTRASONIC_ECHO_PIN
#define MAX_DISTANCE 200 // Maximale Messdistanz (in cm)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setUp() {
  // nichts – wird bei Unity automatisch gemacht
}

void tearDown() {
  // nichts
}

void test_ultrasonic_distance() {
  delay(100); // kurze Pause für Sensor
  unsigned int distance = sonar.ping_cm();

  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    distance = sonar.ping_cm();
    Serial.print("Gemessene Distanz: ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(500);
  }

  TEST_ASSERT_TRUE_MESSAGE(distance <= MAX_DISTANCE, "Distanz ist größer als erwartet!");
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warte auf Serial Monitor
  
  UNITY_BEGIN();
  RUN_TEST(test_ultrasonic_distance);
  UNITY_END();
}

void loop() {
  // nichts
}
