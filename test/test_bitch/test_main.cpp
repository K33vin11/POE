#include <Arduino.h>
#include <unity.h>
#include <BridgePlacer.h>
#include <Config.h> // Für BRIDGE_SERVO_PIN, BRIDGE_DIR_PIN, BRIDGE_DIRECTION

// --- BridgePlacer-Objekt ---
BridgePlacer bridgePlacer(BRIDGE_SERVO_PIN, BRIDGE_DIR_PIN, BRIDGE_DIRECTION);

// --- Setup und Teardown ---
void setUp() {
  bridgePlacer.begin();
}

void tearDown() {
  // nichts nötig
}

// --- Der Test ---
void test_bridge_up_and_down() {


  // Brücke ablegen
  bridgePlacer.bridgeDown(BRIDGE_DOWN_TIME); // 2 Sekunden
  Serial.println("Brücke abgelegt");
  delay(4000); // Kurze Pause
    // Brücke anheben
  bridgePlacer.bridgeUp(BRIDGE_UP_TIME); // 2 Sekunden
  Serial.println("Brücke angehoben");
  delay(4000); // Kurze Pause

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
