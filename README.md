# Autonomes Bergungssystem

Willkommen im offiziellen Repository für unser Autonomes Bergungssystem!  
Dieses Projekt basiert auf einem **Arduino Mega 2560** mit **M5Stack U160-V1.1 Motor-Treibern**, einem **9-Achsen Gyrosensor**, sowie Servo- und Infrarotsensorik.


---

## Projektübersicht

- Omniwheel-Fahrwerk mit 4 unabhängigen Rädern für 360°-Bewegung
- 6 Infrarot-Kantensensoren zur Erkennung von Tischkanten
- Ultraschallsensor für Abstandsmessung und Objekterkennung
- Greifer zum Aufnehmen und Ablegen von Objekten
- Brückenleger zum Ablegen und Anheben einer Brücke
- Live-Positionsbestimmung 📍 (X, Y, Theta) über Gyro
- State Machine zur Ablaufsteuerung (z. B. Brücke platzieren → Objekt holen → zurückfahren)

---

## Klassenstruktur und Hauptfunktionen
### Klassendiagramm
![alternative text](https://github.com/K33vin11/POE/blob/main/KlassenDiagramm.png)


### Robot

Zentrale Steuerklasse des Roboters, kapselt die Hauptkomponenten:

- `DriveSystem` (Bewegung und Orientierung)
- `Gripper` (Greifer-Mechanismus)
- `BridgePlacer` (Brückenleger-Mechanismus)
- `DistanceSensor` (Ultraschall-Abstandsmessung)

**Wichtige Methoden:**

```cpp
begin();             // Initialisiert alle Subsysteme (DriveSystem, Gripper, BridgePlacer, DistanceSensor)
move(float vx, float vy, float omega); // Bewegt den Roboter (Translation + Rotation)
stop();              // Stoppt alle Motoren
stop(const String& direction); // Stoppt mit Bremsen in der angegebenen Richtung ("fw", "bw")
bridgeUp();          // Hebt die Brücke an (mit festgelegter Zeit)
bridgeUp(int extraTime); // Hebt die Brücke an mit zusätzlicher Zeit
bridgeDown();        // Senkt die Brücke ab
gripperDown();       // Senkt den Greifer ab
gripperUp();         // Hebt den Greifer an
gripperClose();      // Schließt den Greifer
gripperOpen();       // Öffnet den Greifer
turn(float targetAngle) : bool; // Dreht den Roboter um einen Winkel
setSpeedLimit(float limit);     // Setzt das Geschwindigkeitslimit
moveToRightEdge(float startOdy) : bool; // Fahre zur rechten Tischkante mit Startorientierung
moveToLeftEdge() : bool;        // Fahre zur linken Tischkante
moveToFrontEdge() : bool;       // Fahre zur vorderen Tischkante
moveToBackEdge() : bool;        // Fahre zur hinteren Tischkante
moveBack(float mills) : bool;   // Fahre rückwärts für angegebene Millisekunden
moveForward(float mills) : bool; // Fahre vorwärts für angegebene Millisekunden
moveAlongRightEdge() : bool;    // Fahre an der rechten Kante entlang
moveAlongRightEdge(int time) : bool; // Für bestimmte Zeit an der rechten Kante entlangfahren
moveAlongRightEdgeBack() : bool; // Fahre rückwärts entlang rechter Kante bis linke Kante erkannt wird
moveAlongRightEdgeBack(int time) : bool; // Rückwärtsfahrt mit Zeitbegrenzung entlang rechter Kante
objectInGripper() : bool;       // Prüft, ob ein Objekt im Greifer liegt (Sensor oder Ultraschall)
