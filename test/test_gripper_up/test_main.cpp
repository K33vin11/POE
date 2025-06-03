#include <Arduino.h>
#include <unity.h>
#include <BridgePlacer.h>
#include <Config.h> // Für BRIDGE_SERVO_PIN, BRIDGE_DIR_PIN, BRIDGE_DIRECTION
#include <Gripper.h>

// --- BridgePlacer-Objekt ---
BridgePlacer bridgePlacer(GRIPPER_MOTOR_PIN, GRIPPER_DIR_PIN, GRIPPER_MOTOR_DIRECTION);

// --- Setup und Teardown ---
void setUp() {
  bridgePlacer.begin();
}

void tearDown() {
  // nichts nötig
}

// --- Der Test ---
void test_bridge_up_and_down() {
  // Brücke anheben
  bridgePlacer.bridgeUp(GRIPPER_MOTOR_UP_TIME); // 2 Sekunden
  Serial.println("gripper angehoben");
  delay(500); // Kurze Pause

  // Brücke ablegen
  bridgePlacer.bridgeDown(GRIPPER_MOTOR_DOWN_TIME); // 2 Sekunden
  Serial.println("gripper abgelegt");

  // Du kannst hier später Sensorprüfungen einbauen, wenn verfügbar
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warten auf Serial-Verbindung
  UNITY_BEGIN();

  RUN_TEST(test_bridge_up_and_down);

  UNITY_END();
}

void loop() {
  // nichts tun
}
