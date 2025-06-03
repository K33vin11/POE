#include <Arduino.h>
#include <Robot.h>
#include <Config.h>
#include <DriveSystem.h>
#include <Wheel.h>
#include <Motor.h>
#include <Encoder.h>
#include <EdgeSensor.h>
#include <Gripper.h>
#include <BridgePlacer.h>
#include <Gyro.h>
#include <UserControl.h>

// ----- Hardware Setup -----
// Erzeuge vier Räder mit jeweiligen I2C-Adressen, Encoder-Pins, Raddurchmesser (in Metern) und Drehrichtung
Wheel wheel1(MOTOR1_I2C_ADDRESS, ENCODER1_A_PIN, ENCODER1_B_PIN, WHEEL_DIAMETER / 100, MOTOR1_DIRECTION);
Wheel wheel2(MOTOR2_I2C_ADDRESS, ENCODER2_A_PIN, ENCODER2_B_PIN, WHEEL_DIAMETER / 100, MOTOR2_DIRECTION);
Wheel wheel3(MOTOR3_I2C_ADDRESS, ENCODER3_A_PIN, ENCODER3_B_PIN, WHEEL_DIAMETER / 100, MOTOR3_DIRECTION);
Wheel wheel4(MOTOR4_I2C_ADDRESS, ENCODER4_A_PIN, ENCODER4_B_PIN, WHEEL_DIAMETER / 100, MOTOR4_DIRECTION);

// Erzeuge sechs Kantensensoren (Infrarot) an definierten Pins
EdgeSensor sensorFrontLeft(IR_FRONT_LEFT_PIN);
EdgeSensor sensorFrontRight(IR_FRONT_RIGHT_PIN);
EdgeSensor sensorLeft(IR_LEFT_PIN);
EdgeSensor sensorRight(IR_RIGHT_PIN);
EdgeSensor sensorBackLeft(IR_BACK_LEFT_PIN);
EdgeSensor sensorBackRight(IR_BACK_RIGHT_PIN);

// Array mit Zeigern auf alle Kantensensoren für einfaches Handling
EdgeSensor *sensors[6] = {&sensorFrontLeft, &sensorFrontRight, &sensorLeft, &sensorRight, &sensorBackLeft, &sensorBackRight};

// Ultraschallsensor zur Entfernungsmessung initialisieren (Trigger, Echo, maximale Distanz)
NewPing sonar(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_MAX_DISTANCE);

// Gyrosensor-Objekt zur Orientierungsmessung
Gyro gyro;

// DriveSystem mit vier Rädern, Kantensensoren und Gyro initialisieren
DriveSystem driveSystem(&wheel1, &wheel2, &wheel3, &wheel4, sensors, &gyro);

// Greifer (Servomotor gesteuert) mit Motor- und Steuerpins
Gripper gripper(GRIPPER_MOTOR_PIN, GRIPPER_DIR_PIN, GRIPPER_MOTOR_DIRECTION, GRIPPER_OPEN_PIN, GRIPPER_CLOSE_PIN);

// Brückenleger (Servomotor gesteuert)
BridgePlacer bridgePlacer(BRIDGE_SERVO_PIN, BRIDGE_DIR_PIN, BRIDGE_DIRECTION);

// Distanzsensor (Ultraschall) für den Roboter
DistanceSensor distanceSensor(ULTRASONIC_TRIG_PIN, ULTRASONIC_ECHO_PIN, ULTRASONIC_MAX_DISTANCE);

// Roboter besteht aus Antriebssystem, Greifer, Brückenleger und Distanzsensor
Robot robot(&driveSystem, &gripper, &bridgePlacer, &distanceSensor);

// Benutzereingaben: Start, Stop und Reset Taster
UserControl userButtons(BUTTON_START_PIN);

// ----- Variablen -----
// Steuerung des Roboters: Ist der Roboter gerade aktiv?
bool running = false;
// Flag für Resetanforderung
bool resetRequested = false;

// Anzahl der ausgelieferten Objekte (Zähler)
int deliveredObjects = 0;

// Aktueller Zustand der State Machine, 100 ist Startzustand
int currentState = 100;

// Geschwindigkeitseinstellung, Standard ca. 40% (0.4)
float speedLimit = 0.4;

// Startorientierung (Yaw) des Gyrosensors
float startOdy = 0.0;

void setup()
{
    // I2C initialisieren
    Wire.begin();
    gyro.begin(); // Gyro initialisieren

    Serial.begin(9600);
    Serial.println("Starte Roboter...");

    robot.begin(); // Alle Subsysteme starten (DriveSystem, Greifer, Brückenleger)
    delay(1000);

    userButtons.begin();  // Taster initialisieren
    bridgePlacer.begin(); // Brückenleger initialisieren

    // Geschwindigkeitslimit im Antrieb setzen
    robot.setSpeedLimit(speedLimit);
    delay(2000);

    robot.stop("fw"); // Roboter anhalten
    gripper.begin();        // Greifer initialisieren
    delay(1000);

    // Pin zum Erkennen von Objekten mit dem Greifer als Eingang definieren
    pinMode(GRIPPER_OBJECT_PIN, INPUT);

    // Greifer erst schliessen, dann öffnen, dann hochfahren (Startposition)
    robot.gripperClose();
    gripper.open(GRIPPER_OPEN_TIME);
    gripper.Up(GRIPPER_MOTOR_UP_TIME);

    // Brücke hochfahren (Startposition)
        robot.bridgeUp(); // Brücke  hoch

    Serial.println("Roboter bereit, warte auf START");
}

void loop()
{
    // Debug-Ausgaben zur aktuellen State und Orientierung
    Serial.println("Aktueller Zustand: " + String(currentState));
    Serial.println("Aktuelle Orientierung: " + String(gyro.getOrientation()));
    Serial.println("Burim isch der Bescht!"); // Humorvolle Statusmeldung

    delay(100); // Kurze Pause für seriellen Output

    // Startknopf gedrückt? Dann State auf 0 setzen und Startorientierung merken
    if (userButtons.startPressed())
    {
        Serial.println("Start gedrückt");
        currentState = 0;
        startOdy = gyro.getOrientation();
        Serial.println("Start Ody gesetzt: " + String(startOdy));
    }

    // State Machine mit vielen Fällen für unterschiedliche Robotertätigkeiten
    switch (currentState)
    {
    case 0:
        // Initialisierung und Startsignal
        Serial.println("Starte");
        delay(2000);                     // kurze Pause vor dem Start
        currentState = currentState + 1; // weiter zum nächsten Schritt
        break;

    case 1:
        // Fahre zur rechten Kante
        robot.setSpeedLimit(0.28);          // langsame Geschwindigkeit für präzise Fahrt
        if (robot.moveToRightEdge(startOdy)) // rechts an die Kante fahren
        {
            robot.setSpeedLimit(0.35); // leicht schneller nach Erreichen der Kante
            delay(200);
            currentState = currentState + 1;
        }
        break;

    case 2:
        // Fahre rückwärts entlang rechter Kante
        robot.setSpeedLimit(0.3);
        if (robot.moveAlongRightEdgeBack()) // rückwärts entlang rechter Kante
        {
            delay(200);
            currentState = currentState + 1;
        }
        break;

    case 3:
        // Fahre nach vorne zur hinteren Tischkante
        robot.setSpeedLimit(0.35);
        if (robot.moveForward(500)) // 500 mm vorwärts fahren
        {
            Serial.println("hinten erreicht");
            delay(200);
            currentState = currentState + 1;
        }
        break;

    case 4:
        // Brücke absenken
        Serial.println("Platziere brücke");
        robot.bridgeDown(); // Brücke  absenken
        delay(2000);
        currentState = currentState + 1;
        break;

    case 5:
        // Fahre weiter nach hinten
        Serial.println("Fahre nach hinten");
        if (robot.moveForward(550)) // 550 mm vorwärts (weiter hinten)
        {
            Serial.println("hinten erreicht");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 6:
        // Brücke wieder hochfahren
        Serial.println("Gable hoch");
        robot.bridgeUp(); // Brücke  hoch
        delay(2000);
        currentState = currentState + 1;
        break;

    case 7:
        // Fahre rückwärts über die Brücke
        Serial.println("über brücke fahren");
        if (robot.moveBack(2300, startOdy)) // 2300 mm rückwärts mit Orientierungskontrolle
        {
            Serial.println("auf anderer Seite angekommen");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 8:
        // Leichte Linkskorrektur (Positionierung nach links)
        Serial.println("fahre einwenig nach links");
        robot.setSpeedLimit(0.4);  // Geschwindigkeit setzen
        robot.move(0.0, 1.0, 0.0); // seitwärts nach links fahren
        delay(500);
        robot.move(0, 0, 0);      // stoppen
        robot.setSpeedLimit(0.4); // Geschwindigkeit zurücksetzen
        currentState = currentState + 1;
        break;

    case 9:
        // 90° Drehung nach links
        robot.setSpeedLimit(0.4);
        robot.move(0, 0, 1.0); // Roboter um 90 Grad links drehen (Drehgeschwindigkeit 1.0)
        delay(1200);
        robot.move(0, 0, 0); // stoppen
        delay(1000);
        currentState = currentState + 1;
        break;

    case 10:
        // Greifer senken
        Serial.println("Sonar runter");
        robot.gripperDown(); // Greifer absenken, z.B. für Sonar
        delay(2000);
        Serial.println("Sonar runter fertig");
        currentState = currentState + 1;
        break;

    case 11:
        // Objektsuche mit erster Annäherung
        robot.setSpeedLimit(0.38);
        Serial.println(" Starte Objektsuche und Annäherung...");
        while (true)
        {
            robot.move(0.0, 0.00, -0.7); // leicht nach rechts drehen und schieben
            if (sonar.ping_cm() > 0)
            {
                robot.move(0, 0.00, 1); // vorwärts mit leichter Rechtskorrektur
                delay(150);
                robot.move(0, 0, 0);
                Serial.println("mitte erreicht");
                delay(1000);
                currentState = currentState + 1;
            }
            break;
        }
        break;

    case 12:
    {
        // Feinjustierung zur Objektmitte
        robot.setSpeedLimit(0.30);
        static bool objectSearchStarted = false;
        static float correction = 0.1;

        if (!objectSearchStarted)
        {
            Serial.println("Starte Objektsuche und Annäherung...");
            objectSearchStarted = true;
        }

        long distance = sonar.ping_cm();
        Serial.println("Distanz: " + String(distance) + " cm");

        // Korrektur der Bewegung abhängig von Sonar-Entfernung
        if (distance > 0)
        {
            correction = 0.005;
            robot.move(0.7, 0.00, correction); // leicht nach links korrigieren
            Serial.println("Korrigiere nach links");
            delay(10);
        }
        else
        {
            correction = -0.08;
            robot.move(0.7, 0.00, correction); // leicht nach rechts korrigieren
            Serial.println("Korrigiere nach rechts");
            delay(10);
        }

        // Prüfe, ob Objekt erreicht wurde (Greifer-Sensor oder Sonar < 6 cm)
        if ((digitalRead(GRIPPER_OBJECT_PIN) == LOW) || (sonar.ping_cm() < 6 && sonar.ping_cm() > 0))
        {
            robot.stop("fw");
            Serial.println("Objekt erreicht!");
            delay(1000);
            currentState++;              // nächster Schritt
            objectSearchStarted = false; // Reset für nächsten Lauf
        }

        break;
    }

    case 13:
        // Objekt greifen
        Serial.println("Greife Objekt");
        Serial.println("Greifer abgesenkt");
        delay(1000);
        robot.gripperClose(); // Greifer schliessen
        Serial.println("Greifer geschlossen");
        delay(1000);
        robot.gripperUp(); // Greifer hochfahren
        Serial.println("ICh heb ab");
        delay(1000);
        currentState = currentState + 1;
        break;

    case 14:
        // Zurückdrehen auf Startausrichtung
        Serial.println("Drehe auf 0");
        robot.setSpeedLimit(0.38);
        if (robot.turn(startOdy - gyro.getOrientation()))
        {
            robot.setSpeedLimit(0.4);
            Serial.println("Gedreht auf 0 Grad");
            delay(1000);
            robot.setSpeedLimit(speedLimit);
            currentState = currentState + 1;
        }
        break;

    case 15:
        // Zur rechten Kante fahren
        robot.setSpeedLimit(0.3);
        Serial.println("Fahre an rechte Kante");
        Serial.println("Start Ody: " + String(startOdy));
        if (robot.moveToRightEdge(startOdy))
        {
            Serial.println("Kante erreicht");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 16:
        // An rechter Kante entlangfahren
        robot.setSpeedLimit(0.30);
        Serial.println("Fahre an rechter Kante entlang");
        robot.moveAlongRightEdge(2500);
        if (robot.moveAlongRightEdge())
        {
            robot.move(-1, 0, 0);
            delay(100);
            robot.move(0, 0, 0);
            currentState++;
        }
        break;

    case 17:
        // Zurückfahren zum Ablagepunkt
        Serial.println("Fahre nach hinten");
        robot.setSpeedLimit(0.3);
        if (robot.moveBack(600))
        {
            robot.move(0, 0, 0);
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 18:
        // Objekt ablegen
        robot.gripperDown();
        robot.gripperOpen();
        delay(1000);
        robot.gripperUp();
        delay(100);
        Serial.println("Objekt abgelegt");
        currentState = currentState + 1;
        break;

    case 19:
        // Zweiter Durchlauf – zur rechten Kante
        robot.setSpeedLimit(0.28);
        Serial.println("Fahre an rechte Kante");
        Serial.println("Start Ody: " + String(startOdy));
        if (robot.moveToRightEdge(startOdy))
        {
            robot.setSpeedLimit(0.4);
            Serial.println("Kante erreicht");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 20:
        // Über die Brücke fahren
        Serial.println("über brücke fahren");
        if (robot.moveBack(2800, startOdy))
        {
            Serial.println("auf anderer Seite angekommen");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 21:
        // Leichte Linkskorrektur
        Serial.println("fahre einwenig nach links");
        robot.setSpeedLimit(0.4);
        robot.move(0.0, 1.0, 0.0);
        delay(500);
        robot.move(0, 0, 0);
        robot.setSpeedLimit(0.4);
        currentState = currentState + 1;
        break;

    case 22:
        // 90° Drehung nach links
        robot.setSpeedLimit(0.4);
        robot.move(0, 0, 1.0);
        delay(1200);
        robot.move(0, 0, 0);
        delay(1000);
        currentState = currentState + 1;
        break;

    case 23:
        // Greifer senken
        Serial.println("Sonar runter");
        robot.gripperDown();
        delay(2000);
        Serial.println("Sonar runter fertig");
        currentState = currentState + 1;
        break;

    case 24:
        // Objektsuche mit erster Annäherung
        robot.setSpeedLimit(0.4);
        Serial.println(" Starte Objektsuche und Annäherung...");
        while (true)
        {
            robot.move(0.0, 0.00, -0.55);
            if (sonar.ping_cm() > 0)
            {
                robot.move(0, 0.00, 0.55);
                delay(100);
                robot.move(0, 0, 0);
                Serial.println("Objekt erkannt!");
                delay(2000);
                currentState = currentState + 1;
                break;
            }
        }
        break;

    case 25:
    {
        // Feinjustierung zur Objektmitte
        robot.setSpeedLimit(0.35);
        static bool objectSearchStarted = false;
        static float correction = 0.1;

        if (!objectSearchStarted)
        {
            Serial.println("Starte Objektsuche und Annäherung...");
            objectSearchStarted = true;
        }

        long distance = sonar.ping_cm();
        Serial.println("Distanz: " + String(distance) + " cm");

        if (distance > 0)
        {
            correction = 0.005;
            robot.move(0.7, 0.00, correction);
            Serial.println("Korrigiere nach links");
            delay(50);
        }
        else
        {
            correction = -0.07;
            robot.move(0.7, 0.00, correction);
            Serial.println("Korrigiere nach rechts");
            delay(50);
        }

        if (robot.objectInGripper())
        {
            Serial.println("Objekt erkannt!");
            robot.stop("fw");
            delay(1000);
            currentState = currentState + 1;
        }
        break;
    }

    case 26:
        // Objekt greifen
        Serial.println("Greife Objekt");
        Serial.println("Greifer abgesenkt");
        delay(1000);
        robot.gripperClose();
        Serial.println("Greifer geschlossen");
        delay(1000);
        robot.gripperUp();
        Serial.println("ICh heb ab");
        delay(1000);
        currentState = currentState + 1;
        break;

    case 27:
        // Zurückdrehen auf Startausrichtung
        Serial.println("Drehe auf 0");
        robot.setSpeedLimit(0.38);
        if (robot.turn(startOdy - gyro.getOrientation()))
        {
            robot.setSpeedLimit(0.4);
            Serial.println("Gedreht auf 0 Grad");
            delay(1000);
            robot.setSpeedLimit(speedLimit);
            currentState = currentState + 1;
        }
        break;

    case 28:
        // Zur rechten Kante fahren
        robot.setSpeedLimit(0.3);
        Serial.println("Fahre an rechte Kante");
        Serial.println("Start Ody: " + String(startOdy));
        if (robot.moveToRightEdge(startOdy))
        {
            Serial.println("Kante erreicht");
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 29:
        // An rechter Kante entlangfahren
        robot.setSpeedLimit(0.30);
        Serial.println("Fahre an rechter Kante entlang");
        robot.moveAlongRightEdge(2500);
        Serial.println("Fahre bis zum ende des tisches");
        if (robot.moveAlongRightEdge())
        {
            robot.move(0, 0, 0);
            currentState++;
        }
        break;

    case 30:
        // Zurückfahren zum Ablagepunkt
        Serial.println("Fahre nach hinten");
        if (robot.moveBack(600))
        {
            robot.move(0, 0, 0);
            delay(1000);
            currentState = currentState + 1;
        }
        break;

    case 31:
        // Objekt ablegen
        robot.gripperDown();
        robot.gripperOpen();
        delay(1000);
        robot.gripperUp();
        delay(100);
        Serial.println("Objekt abgelegt");
        currentState = currentState + 1;
        break;

    case 32:
        // Zur rechten Kante fahren
        robot.setSpeedLimit(0.28);
        Serial.println("Fahre an rechte Kante");
        Serial.println("Start Ody: " + String(startOdy));
        if (robot.moveToRightEdge(startOdy))
        {
            delay(500);
            currentState = currentState + 1;
            break;
        }

    case 33:
        // Brücke absenken
        robot.bridgeDown(); // Brücke  absenken
        delay(2000);
        currentState = currentState + 1;
        break;

    case 34:
        // Rückfahrt entlang rechter Kante
        robot.setSpeedLimit(0.35);
        if (robot.moveAlongRightEdgeBack(1700))
        {
            robot.move(-1, 0, 0);
            delay(100);
            robot.move(0, 0, 0);
            Serial.println("Vorne links Kante erkanntStop");
            delay(1000);
            currentState++;
        }
        break;

    case 35:
        // Brücke hochfahren
        Serial.println("Brücke hoch");
        robot.bridgeUp(500); // Brücke  hoch mit extra Zeit
        delay(4000);
        currentState = currentState + 1;
        break;

    case 36:
        // Nach vorne zur Startposition fahren
        Serial.println("Fahre nach vorne");
        if (robot.moveForward(1200))
        {
            robot.move(0, 0, 0);
            delay(1000);
            currentState = 100; // zurück zum Wartezustand (Start)
        }
        break;

    case 100:
        // Warte auf neuen Startbefehl
        if (userButtons.startPressed())
        {
            currentState = 0; // Starte neuen Ablauf
        }
        break;
    }
}
