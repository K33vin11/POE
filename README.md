# Autonomes Bergungssystem

Willkommen im offiziellen Repository für unser Autonomes Bergungssystem!  
Dieses Projekt basiert auf einem **Arduino Mega 2560** mit **M5Stack U160-V1.1 Motor-Treibern**, einem **9-Achsen Gyrosensor**, sowie Servo- und Infrarotsensorik.


---

## 🔹 Projektübersicht

- Omniwheel-Fahrwerk mit 4 unabhängigen Rädern für 360°-Bewegung
- 6 Infrarot-Kantensensoren zur Erkennung von Tischkanten
- Ultraschallsensor für Abstandsmessung und Objekterkennung
- Greifer zum Aufnehmen und Ablegen von Objekten
- Brückenleger zum Ablegen und Anheben einer Brücke
- Live-Positionsbestimmung 📍 (X, Y, Theta) über Gyro
- State Machine zur Ablaufsteuerung (z. B. Brücke platzieren → Objekt holen → zurückfahren)

---

## 🔹 Klassenstruktur und Hauptfunktionen
![Klassendiagramm](https://www.plantuml.com/plantuml/png/pLRVRnit37xtNw7794k0OHY6OGz3ysCj0gIBKITOYs8Ov1DxrgeJ3nJFYJViVv-FacrUxEQMxMKljaX-fZwA_6IzIco9koPCqcSVMqksCLMmAPqpavzt3DSLnfd_0ypNaub4p3kS8vi_9yQScIloIvYj4aDpOjpEaE0RycqBT6Bgl13d1NbNmpJOge_CbPKJ-iGsLZ23c70DSDhk8L-PETG-7XtBki4b77qAQDai7qzDMQqsAsoWjXcP6DkZ6PEFjN6Ue6AFKKA5n7rxzCJqaGqyCjaxtu2ENU538R0fRTyZwPHz6J31q7NRGYmUxYYMEjXI3NmUwm37vWSpHmov0E1P2-1kVEEvO4E_tft47RxpzO9_SZKKWBJtrgrqfWAzWKyPUI3scZ1-CNXXgy-7OtsaRDpu4D8Uu3NIWoNtBEOyOAntTOp4fKdi6nZ12RL_WFbola7qr3nplZZ_7Ig-ZcN-Tzl-LIIaPLE4zCi289oOX_uhVVpsLt7sf3P2WEqwVVmkXz-i2DVYMd-AcHcrcDuFpG_AHF8GsVQphxofdwj_hor1NYBreZhmW-FnyIto75Eg7i2NJwvMntEQWUX65JCo-AC0FJz5wo_LIW9BrI9Vf-guBWQW6lYo8uB8jxlE2tz9AvYkTPPXYidde3W94l2jmmQOLkBJoMwmscNQ61c3TpYuoDwV6cr--F_hVUGwrZeMrHO5lqL6Cat_cSKRAtH01lAtaflpjW46qeeRQ4H8YIXV8euQUFFl-HQNu5IRjnrTPzcI5yA5VESZV_yR6_zDTUuSGKgzMwerFYw1-7c245qEb4GxRgTcY5K5ZnUcs9RZp7mbP-lZkJOknEWxRDek94ZRO6d1TNJmUB0kpMbXemjm7Hc8kfQtUh1kAbdxOvLTzAdAkhAXwe9bsBH1TM1JXxhJXwNiyN30K0qriSrIdbOwHG9QujJ7uf0EQ1DRYDEjLGLC8AOaxi5x1EHDivM7kExReKEo3bqvjV9jEUkNtpZn_y1DRrfQiN_aJBxMkqAr41PAfi1v2kyM16c1mKdmPtXOu-eJap3QVMOry7xxrRFWIhA2EzJK5M5Xc1bKUrVtWUrq-D30tIUWIunC68OJDsDxOFnJxvqIfFI4Otxsdvtzg5ylobj6KdbKmxLxs9B9HAMJk5ogJxrADqz3ScwJYVnCRBdOb5skZiahYAvhmjy0)





### 🧠 Robot

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
