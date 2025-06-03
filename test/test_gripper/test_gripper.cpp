#include <Arduino.h>
#include <unity.h>
#include <Gripper.h>
#include <Config.h> // Für den GRIPPER_SERVO_PIN

// --- Gripper-Objekt ---
Gripper gripper(GRIPPER_MOTOR_PIN, GRIPPER_DIR_PIN, GRIPPER_MOTOR_DIRECTION, GRIPPER_OPEN_PIN, GRIPPER_CLOSE_PIN);

// --- Setup und Teardown ---
void setUp() {
  gripper.begin();
}

void tearDown() {
  // nichts nötig
}

// --- Der Test ---
void test_gripper_open_and_close() {


  // 2. Schliessen
  gripper.close(GRIPPER_CLOSE_TIME);
  Serial.println("Gripper geschlossen");
  delay(3000); // 1 Sekunde warten

    // 1. Öffnen
  gripper.open(GRIPPER_OPEN_TIME);
  Serial.println("Gripper geöffnet");
  delay(3000); // 1 Sekunde warten

  // (Optional könntest du hier später noch Sensoren prüfen, z.B. ob wirklich ein Objekt gegriffen wurde)
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warten auf Serial Verbindung
  UNITY_BEGIN();
  
  RUN_TEST(test_gripper_open_and_close);
  
  UNITY_END();
}

void loop() {
  // nichts tun
}
