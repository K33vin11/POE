#include <Arduino.h>
#include <unity.h>

void setUp() {
  // Vorbereitung, wenn nötig
}

void tearDown() {
  // Aufräumen, wenn nötig
}

void test_always_ok() {
  TEST_ASSERT_TRUE_MESSAGE(true, "Dieser Test wird immer OK sein.");
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warten auf Serial Monitor
  UNITY_BEGIN();
  
  RUN_TEST(test_always_ok);
  
  UNITY_END();
}

void loop() {
  // leer
}
