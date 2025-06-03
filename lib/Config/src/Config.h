#ifndef CONFIG_H
#define CONFIG_H

// Buttons
#define BUTTON_START_PIN 42

// Motor
// I2C Adressen der 4 Motor Driver
#define MOTOR1_I2C_ADDRESS 0x20 // Vorne Links
#define MOTOR2_I2C_ADDRESS 0x21 // Vorne Rechts
#define MOTOR3_I2C_ADDRESS 0x22 // Hinten Links
#define MOTOR4_I2C_ADDRESS 0x24 // Hinten Rechts

#define MOTOR1_DIRECTION false
#define MOTOR2_DIRECTION true
#define MOTOR3_DIRECTION false
#define MOTOR4_DIRECTION true

// Wheel
#define WHEEL_DIAMETER 9.7 // Durchmesser der Räder in cm
#define ENCODER_CPR 341.2 // Counts Per Revolution 
#define PI_PI 3.14159265358979323846 // Pi für Berechnungen

// Encoder Pins
#define ENCODER1_A_PIN 18 // Interrupt 0 Gelb
#define ENCODER1_B_PIN 23 // Weiss
#define ENCODER2_A_PIN 19 // Interrupt 1 Gelb
#define ENCODER2_B_PIN 24 // Weiss
#define ENCODER3_A_PIN 2 // Interrupt 5 Gelb
#define ENCODER3_B_PIN 26 // Weiss
#define ENCODER4_A_PIN 3 // Interrupt 4 Gelb
#define ENCODER4_B_PIN 28 // Weiss

// IR Edge Sensors
#define IR_FRONT_LEFT_PIN 31 //
#define IR_FRONT_RIGHT_PIN 29
#define IR_LEFT_PIN 33
#define IR_RIGHT_PIN 27
#define IR_BACK_LEFT_PIN 23
#define IR_BACK_RIGHT_PIN 25 


// Gripper 
#define GRIPPER_SERVO_PIN 50 // Pin für Greifer Servo
#define GRIPPER_OPEN_POSITION 0 // Servo Position für offen
#define GRIPPER_CLOSE_POSITION 720 // Servo Position für geschlossen

#define GRIPPER_OPEN_PIN 40 // Pin für Greifer öffnen
#define GRIPPER_CLOSE_PIN 41 // Pin für Greifer schließen

#define GRIPPER_OPEN_TIME 1000 // Zeit in ms, wie lange der Greifer öffnet
#define GRIPPER_CLOSE_TIME 1000 // Zeit in ms, wie lange der Greifer schliesst
#define GRIPPER_MOTOR_PIN 49 // blau 
#define GRIPPER_DIR_PIN 47 


#define GRIPPER_MOTOR_DIRECTION true // Richtung des Greifer-Motors (true = vorwärts, false = rückwärts)
#define GRIPPER_MOTOR_DOWN_TIME 2000  // Zeit in ms, wie lange der Greifer-Motor nach unten fährt
#define GRIPPER_MOTOR_UP_TIME 2400  // Zeit in ms, wie lange der Greifer-Motor nach oben fährt

#define GRIPPER_OBJECT_PIN 22

// Bridge 

#define BRIDGE_SERVO_PIN 51 // grün
#define BRIDGE_DIR_PIN 47 //rot
#define BRIDGE_DIRECTION true // Richtung des Brücken-Motors 
#define BRIDGE_DOWN_TIME 3450 // Zeit in ms, wie lange die Brücke unten bleibt
#define BRIDGE_UP_TIME 4000 // Zeit in ms, wie lange die Brücke oben bleibt


//Gyro

#define GYRO_PIN 18 // von UNO 3 auf 18


// Ultrasonic (falls gebraucht)
#define ULTRASONIC_TRIG_PIN 35
#define ULTRASONIC_ECHO_PIN 36
#define ULTRASONIC_MAX_DISTANCE 80 //cm

// Korrekturen
#define MIN_ABWEICHUNG 0.01 // Minimalabweichung für Korrektur
#define KORR_VALUE 0.2 // Korrekturwert für Abweichung

#define ANGLE_DIFF_BEFORE_STOP 60 // Winkelabweichung vor dem Stoppen des Roboters

#endif
