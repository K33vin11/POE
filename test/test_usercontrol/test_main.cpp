#include <Arduino.h>
#include "UserControl.h"
#include "config.h"

UserControl userControl(BUTTON_START_PIN, BUTTON_STOP_PIN, BUTTON_RESET_PIN);

void usertest() {
    Serial.println("Klicke Start Button...");

    // Warte auf Start
    while (!userControl.startPressed()) {
        Serial.println("Noch nicht gedrückt...");
        delay(100);
    }

    Serial.println("Start Button gedrückt!");

    // Warte auf Reset
    Serial.println("Klicke Reset Button...");
    while (!userControl.resetPressed()) {
        Serial.println("Noch nicht gedrückt...");
        delay(100);
    }

    Serial.println("Reset Button gedrückt!");
}

void setup() {
    Serial.begin(9600);
    userControl.begin();
    delay(1000); // Zeit für seriellen Monitor
    //usertest();
}

void loop() {
    if (userControl.startPressed()) {
        Serial.println("Start Button gedrückt!");
        delay(1000); // Kurze Pause
    }
    if (userControl.stopPressed()) {
        Serial.println("Stop Button gedrückt!");
        delay(1000); // Kurze Pause
    }
    if (userControl.resetPressed()) {
        Serial.println("Reset Button gedrückt!");
        delay(1000); // Kurze Pause
    }

    // nichts
}
