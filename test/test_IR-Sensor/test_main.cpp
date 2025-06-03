#include <Arduino.h>
#include <unity.h>
#include <EdgeSensor.h>
#include <Config.h> // Für die Pins

// --- Einzelner Sensor ---
EdgeSensor frontLeft(IR_FRONT_LEFT_PIN);

// --- Setup und Teardown ---
void setUp() {
  frontLeft.begin();
}

void tearDown() {
  // nichts nötig
}

// --- Der Test ---
void test_lift_robot_for_front_left_sensor() {
  // 1. Prüfen: Sensor muss zuerst LOW sein (Boden erkannt)
  bool edgeDetected = frontLeft.detectEdge();
  TEST_ASSERT_TRUE_MESSAGE(edgeDetected, "Front-Left Sensor erkennt keine Kante am Anfang (Tisch erwartet)!");

  // 2. Aufforderung: Roboter anheben
  Serial.println(">>> Bitte den Roboter jetzt hochheben! <<<");
  delay(5000); // 5 Sekunden Zeit geben

  // 3. Prüfen: Sensor soll HIGH sein (kein Boden erkannt)
  edgeDetected = frontLeft.detectEdge();
  TEST_ASSERT_FALSE_MESSAGE(edgeDetected, "Front-Left Sensor erkennt immer noch Boden! Roboter korrekt angehoben?");
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warten auf Serial Verbindung
  UNITY_BEGIN();
  
  RUN_TEST(test_lift_robot_for_front_left_sensor);
  
  UNITY_END();
}

void loop() {
  // nichts tun
}




/*
#include <Arduino.h>
#include <unity.h>
#include "EdgeSensor.h"
#include "Config.h" // Für die Pins

// --- Sensor-Objekte ---
EdgeSensor frontLeft(IR_FRONT_LEFT_PIN);
EdgeSensor frontRight(IR_FRONT_RIGHT_PIN);
EdgeSensor left(IR_LEFT_PIN);
EdgeSensor right(IR_RIGHT_PIN);
EdgeSensor back(IR_BACK_PIN);

// --- Array für alle Sensoren ---
EdgeSensor* sensors[5] = {
  &frontLeft,
  &frontRight,
  &left,
  &right,
  &back,
};

// --- Setup und Teardown ---
void setUp() {
  for (int i = 0; i < 5; i++) {
    sensors[i]->begin();
  }
}

void tearDown() {
  // nichts nötig
}

// --- Der Test ---
void test_lift_robot_for_ir_sensors() {
  // 1. Prüfen: Alle Sensoren müssen zuerst LOW sein (Boden erkannt)
  for (int i = 0; i < 5; i++) {
    bool edgeDetected = sensors[i]->detectEdge();
    TEST_ASSERT_TRUE_MESSAGE(edgeDetected, "Sensor erkennt keine Kante am Anfang (Tisch erwartet)!");
  }

  // 2. Aufforderung: Roboter anheben
  Serial.println(">>> Bitte den Roboter jetzt hochheben! <<<");
  delay(5000); // 5 Sekunden Zeit geben

  // 3. Prüfen: Alle Sensoren sollen HIGH sein (kein Boden erkannt)
  for (int i = 0; i < 5; i++) {
    bool edgeDetected = sensors[i]->detectEdge();
    TEST_ASSERT_FALSE_MESSAGE(edgeDetected, "Sensor erkennt immer noch Boden! Roboter korrekt angehoben?");
  }
}

void setup() {
  Serial.begin(9600);
  delay(2000); // Warten auf Serial Verbindung
  UNITY_BEGIN();
  
  RUN_TEST(test_lift_robot_for_ir_sensors);
  
  UNITY_END();
}

void loop() {
  // nichts tun
}

*/
