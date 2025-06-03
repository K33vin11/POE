#include <DriveSystem.h>
#include <math.h>
#include <Config.h>

// Konstruktor: Initialisiert Räder, Sensoren und Gyro mit Startwerten für Position und Orientierung
DriveSystem::DriveSystem(Wheel *w1, Wheel *w2, Wheel *w3, Wheel *w4, EdgeSensor *sensors[6], Gyro *gyroSensor)
    : posX(0.0), posY(0.0), orientation(0.0)
{
    speed = 1.0;               // Standardgeschwindigkeit auf 100% setzen
    wheels[0] = w1;            // Rad vorne links zuweisen
    wheels[1] = w2;            // Rad vorne rechts zuweisen
    wheels[2] = w3;            // Rad hinten links zuweisen
    wheels[3] = w4;            // Rad hinten rechts zuweisen

    // Kantensensoren in Array speichern (6 Sensoren)
    for (int i = 0; i < 6; i++)
    {
        edgeSensors[i] = sensors[i];
    }
    gyro = gyroSensor;         // Gyrosensor speichern
}

// Initialisiert das DriveSystem (Sensoren, evtl. Gyro)
void DriveSystem::begin()
{
    // gyro->begin();  // Gyro-Init auskommentiert, evtl. an anderer Stelle

    delay(1000); // Warten auf Gyro-Initialisierung

    Serial.println("DriveSystem initialisieren...");

    /* Räder-Initialisierung aktuell auskommentiert
    for (int i = 0; i < 4; i++) {
        wheels[i]->begin();
    }
    */

    // Kantensensoren initialisieren (nur 5 von 6 Sensoren?)
    for (int i = 0; i < 5; i++)
    {
        edgeSensors[i]->begin();
    }

    Serial.println("DriveSystem bereit!");
}

// Bewegung: Omniwheel-Kinematik, Berechnung der Raddrehzahlen und Setzen
void DriveSystem::move(float vx, float vy, float omega)
{
    // Berechnung der Einzelradgeschwindigkeiten
    float w1 = vx - vy - omega;
    float w2 = vx + vy + omega;
    float w3 = vx + vy - omega;
    float w4 = vx - vy + omega;

    // Maximalwert für Normierung ermitteln
    float maxVal = max(max(abs(w1), abs(w2)), max(abs(w3), abs(w4)));

    // Normalisieren, falls Geschwindigkeit größer als 1.0
    if (maxVal > 1.0)
    {
        w1 /= maxVal;
        w2 /= maxVal;
        w3 /= maxVal;
        w4 /= maxVal;
    }

    // Geschwindigkeiten in PWM-Werte (0-255) umrechnen und mit Speed-Faktor multiplizieren
    wheels[0]->setSpeed(w1 * 255 * speed);
    wheels[1]->setSpeed(w2 * 255 * speed);
    wheels[2]->setSpeed(w3 * 255 * speed);
    wheels[3]->setSpeed(w4 * 255 * speed);
}

// Bewegung mit Gyro-Korrektur, damit Roboter geradeaus fährt
void DriveSystem::moveCorrect(float vx, float vy)
{
    static float targetHeading = NAN; // Zielwinkel speichern

    // Nur wenn eine Bewegung (vx oder vy ungleich 0) stattfindet
    if (vx != 0.0 || vy != 0.0)
    {
        // Zielwinkel merken, wenn noch keiner gesetzt ist
        if (isnan(targetHeading))
        {
            targetHeading = gyro->getOrientation(); // Startwinkel merken
        }

        float current = gyro->getOrientation();

        // Winkelabweichung auf -180 bis 180 Grad normalisieren
        float diff = fmod((current - targetHeading + 540.0), 360.0) - 180.0;

        // Kleine Abweichungen ignorieren (tot zone)
        if (abs(diff) < MIN_ABWEICHUNG)
            diff = 0.0;

        // Omega-Wert (Drehgeschwindigkeit) begrenzen
        float omega = constrain(diff / 10.0, -KORR_VALUE, KORR_VALUE);

        // Bewegung mit Korrektur aufrufen
        move(vx, vy, omega);
    }
    else
    {
        // Bewegung gestoppt, Zielwinkel zurücksetzen
        targetHeading = NAN;
        move(0.0, 0.0, 0.0);
    }
}

// Setzt das Speed-Limit mit kleinem Zuschlag zur Reserve
void DriveSystem::setSpeedLimit(float limit)
{
    limit = limit + 0.05; // Reserve hinzufügen
    speed = constrain(limit, 0.0, 1.0); // Auf Bereich [0..1] begrenzen
}

// Stoppt alle Räder mit Richtungsangabe (z.B. "fw", "bw")
void DriveSystem::stop(String direction)
{
    for (int i = 0; i < 4; i++)
    {
        wheels[i]->stop(direction);
    }
}

// Stoppt alle Räder sofort (ohne Richtung)
void DriveSystem::stop()
{
    for (int i = 0; i < 4; i++)
    {
        wheels[i]->stop();
    }
}

// Fahrt rückwärts bis hintere Kante erkannt wird, mit Gyro-Korrektur
bool DriveSystem::moveToBackEdge()
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");
    startwinkel = gyro->getOrientation(); // Startwinkel vor Schleife merken

    unsigned long startTime = millis();
    while (millis() - startTime < 30000) // max. 30 Sekunden fahren
    {
        // Winkelabweichung berechnen und skalieren
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0; // Kleine Abweichungen ignorieren
        }

        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE); // Begrenzen

        // Rückwärts fahren mit Winkelkorrektur
        move(-1.0, 0.0, abweichung);

        delay(20);

        // Prüfen ob einer der hinteren Kantensensoren Kante erkennt
        if (edgeSensors[4]->detectEdge() or edgeSensors[5]->detectEdge())
        {
            stop("bw"); // Stoppen mit Bremsen rückwärts
            return true; // Kante erreicht
        }
    }
    return false; // Timeout ohne Kante
}

/*
float DriveSystem::scanforObject(){
    Serial.println("Starte Objekt-Erkennung...");
    float startwinkel = gyro->getOrientation(); // saubere Initialisierung vor Schleife
    move(0.0, 0.0, 0.4); // Rotation starten (omega > 0 = rechtsdrehung)
    if(ultra)


}

*/

// Fahrt vorwärts bis vordere Kante erkannt wird, mit Gyro-Korrektur
bool DriveSystem::moveToFrontEdge()
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");
    startwinkel = gyro->getOrientation(); // Startwinkel vor Schleife merken

    unsigned long startTime = millis();
    while (millis() - startTime < 30000)
    {
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        move(1.0, 0.0, abweichung);

        delay(20);

        // Prüfen ob vordere Kantensensoren Kante erkennen
        if (edgeSensors[0]->detectEdge() or edgeSensors[1]->detectEdge())
        {
            stop("fw"); // Stoppen mit Bremsen vorwärts
            return true;
        }
    }
    return false;
}

// Fahrt seitlich nach rechts mit Gyro-Korrektur bis rechte Kante erkannt wird
bool DriveSystem::moveToRightEdge()
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");

    startwinkel = gyro->getOrientation(); // Startwinkel vor Schleife merken

    unsigned long startTime = millis();
    while (millis() - startTime < 30000)
    {
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Seitlich nach links fahren (0.0, -1.0)
        move(0.0, -1.0, abweichung);

        delay(20);

        // Prüfen, ob rechte Kantensensoren Kante erkennen
        if (edgeSensors[1]->detectEdge() or edgeSensors[3]->detectEdge() or edgeSensors[5]->detectEdge())
        {
            stop("fw"); // Stoppen mit Bremsen vorwärts
            return true;
        }
    }
    return false;
}

// Fahrt nach rechts bis Kantensensor eine Kante erkennt, mit Startwinkel als Übergabeparameter
bool DriveSystem::moveToRightEdge(float endOdy)
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");

    startwinkel = endOdy; // Startwinkel vom Funktionsaufrufer setzen

    unsigned long startTime = millis();
    while (millis() - startTime < 30000) // max. 30 Sekunden fahren
    {
        // Winkelabweichung berechnen (Skalierung 1/10)
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Abweichung begrenzen
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Seitwärts nach rechts fahren (y-Richtung negativ)
        move(0.0, -1, abweichung);

        delay(20);

        // Prüfen, ob eine rechte Kante erkannt wird (Sensor 1, 3 oder 5)
        if (edgeSensors[1]->detectEdge() or edgeSensors[3]->detectEdge() or edgeSensors[5]->detectEdge())
        {
            // Kurz rückwärts fahren, um Anfahren zu korrigieren
            move(0, 1, 0);
            delay(200);
            // Dann anhalten
            move(0, 0, 0);
            delay(200);
            return true; // Kante erreicht
        }
    }
    return false; // Timeout, Kante nicht gefunden
}

// Fahrt nach links bis Kantensensor Kante erkennt, mit Gyro-Kurskorrektur
bool DriveSystem::moveToLeftEdge()
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");

    startwinkel = gyro->getOrientation(); // Startwinkel merken

    unsigned long startTime = millis();
    while (millis() - startTime < 30000)
    {
        // Winkelabweichung berechnen
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Abweichung begrenzen
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Seitlich nach links fahren (y-Richtung positiv)
        move(0.0, 1.0, abweichung);

        delay(20);

        // Prüfen, ob linke Kante erkannt wird (Sensor 0, 2 oder 4)
        if (edgeSensors[0]->detectEdge() or edgeSensors[2]->detectEdge() or edgeSensors[4]->detectEdge())
        {
            stop("fw"); // Stoppen mit Bremsen vorwärts
            return true; // Kante erreicht
        }
    }
    return false; // Timeout
}

// Rückwärts fahren für eine bestimmte Zeit (millis)
bool DriveSystem::moveBack(float mills)
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");

    startwinkel = gyro->getOrientation(); // Startwinkel merken

    unsigned long startTime = millis();
    while (millis() - startTime < mills) // Fahrtzeit begrenzt
    {
        // Winkelabweichung berechnen
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Abweichung begrenzen
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Rückwärts fahren mit Korrektur
        move(-1.0, 0.0, abweichung);

        delay(20);
    }

    stop("bw"); // Stoppen mit Bremsen rückwärts
    return true;
}

// Rückwärts fahren für eine bestimmte Zeit mit vorgegebenem Startwinkel
bool DriveSystem::moveBack(float mills, float startAngle)
{
    Serial.println("Nimm Orientierung auf und starte Fahrt nach hinten...");

    startwinkel = startAngle; // Startwinkel als Parameter übernehmen

    unsigned long startTime = millis();
    while (millis() - startTime < mills)
    {
        // Winkelabweichung berechnen
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Abweichung begrenzen
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Rückwärts fahren mit Winkelkorrektur
        move(-1.0, 0.0, abweichung);

        delay(20);
    }

    stop("bw"); // Stoppen mit Bremsen rückwärts
    return true;
}

// Vorwärts fahren für eine bestimmte Zeit mit Gyro-Kurskorrektur
bool DriveSystem::moveForward(float mills)
{
    startwinkel = gyro->getOrientation(); // Startwinkel merken

    unsigned long startTime = millis();
    while (millis() - startTime < mills) // Fahrtzeit begrenzt
    {
        // Winkelabweichung berechnen und skalieren
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Begrenzen der Abweichung
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Vorwärts fahren mit Korrektur
        move(1.0, 0.0, abweichung);

        delay(20);
    }

    stop("fw"); // Stoppen mit Bremsen vorwärts
    return true;
}

// Sucht nach einer Brücke, fährt dabei in der angegebenen Richtung (left oder right)
bool DriveSystem::searchBridge(String dirB)
{
    Serial.println("Nimm Orientierung auf und starte suche nach Brücke...");

    startwinkel = gyro->getOrientation(); // Startwinkel vor Beginn merken

    unsigned long startTime = millis();
    while (millis() - startTime < 30000) // maximal 30 Sekunden suchen
    {
        // Winkelabweichung berechnen, skaliert auf 1/10
        abweichung = angleDifference(startwinkel, gyro->getOrientation()) / 10.0;

        // Kleine Abweichungen ignorieren (tot zone)
        if (abs(abweichung) < MIN_ABWEICHUNG)
        {
            abweichung = 0.0;
        }

        // Abweichung begrenzen auf [-KORR_VALUE, KORR_VALUE]
        abweichung = constrain(abweichung, -KORR_VALUE, KORR_VALUE);

        // Fährt seitlich entweder links oder rechts abhängig von dirB
        if (dirB == "left")
        {
            move(0.0, 1.0, abweichung); // nach links
        }
        else if (dirB == "right")
        {
            move(0.0, -1.0, abweichung); // nach rechts
        }

        delay(20);

        // Wenn rechter Rand erkannt wird
        if (edgeSensors[3]->detectEdge() or edgeSensors[5]->detectEdge())
        {
            stop("fw");
            Serial.println("am rechten rand");
            dirB = "left"; // Richtung wechseln
        }

        // Wenn linker Rand erkannt wird
        if (edgeSensors[2]->detectEdge() or edgeSensors[4]->detectEdge())
        {
            stop("fw");
            Serial.println("am linken rand");
            dirB = "right"; // Richtung wechseln
        }

        // Wenn vorne keine Kante erkannt wird, Brücke gefunden
        if (!edgeSensors[0]->detectEdge() and !edgeSensors[1]->detectEdge())
        {
            stop("fw");
            return true; // Brücke erreicht
            break; // Schleife verlassen (technisch hier nicht nötig, da return)
        }
    }
    return false; // Timeout, keine Brücke gefunden
}

// Liefert aktuelle Orientierung des Roboters in Grad (0-360)
float DriveSystem::getCurrentOrientation()
{
    return orientation;
}

// Aktualisiert aktuelle Position basierend auf Radstrecken und Gyro-Orientierung
void DriveSystem::updatePosition()
{
    // Distanz von jedem Rad seit letztem Reset auslesen
    float d1 = wheels[0]->getDistanceMoved(); // vorne links
    float d2 = wheels[1]->getDistanceMoved(); // vorne rechts
    float d3 = wheels[2]->getDistanceMoved(); // hinten links
    float d4 = wheels[3]->getDistanceMoved(); // hinten rechts

    // Reset der Encoder zur Messung der inkrementellen Distanz (Methode ggf. ergänzen)
    for (int i = 0; i < 4; i++)
    {
        wheels[i]->reset();
    }

    // Berechnung der Bewegungen in lokaler Koordinate (vereinfachte Omni-Kinematik)
    float dx = (d1 + d2 + d3 + d4) / 4.0;  // Vorwärtsbewegung
    float dy = (-d1 + d2 + d3 - d4) / 4.0; // Seitwärtsbewegung

    // Umrechnung in globale Koordinaten basierend auf Orientierung
    float angleRad = gyro->getOrientation() * DEG_TO_RAD;
    float globalDx = dx * cos(angleRad) - dy * sin(angleRad);
    float globalDy = dx * sin(angleRad) + dy * cos(angleRad);

    // Position aktualisieren
    posX += globalDx;
    posY += globalDy;

    // Orientierung aktualisieren
    orientation = gyro->getOrientation();
}

// Setzt die aktuelle Position und Orientierung zurück auf 0
void DriveSystem::resetOdometry()
{
    posX = 0.0;
    posY = 0.0;
    orientation = 0.0;
    Serial.println("Odometry zurückgesetzt auf (0,0,0°)");
}

// Liefert die aktuelle Position und Orientierung als Location-Struktur
Location DriveSystem::getCurrentLocation()
{
    Location loc;
    loc.x = posX;
    loc.y = posY;
    loc.theta = orientation;
    return loc;
}

// Fahrt zu einem Zielpunkt (targetX, targetY) in globalen Koordinaten
bool DriveSystem::goTo(float targetX, float targetY)
{
    updatePosition(); // Position aktualisieren

    float dx = targetX - posX;
    float dy = targetY - posY;
    float distance = sqrt(dx * dx + dy * dy);

    if (distance < 1.0) // Ziel in 1 cm Toleranz erreicht
    {
        stop();
        return true; // An Ziel
    }

    // Umrechnung in lokale Koordinaten (Roboterkoordinaten)
    float thetaRad = orientation * DEG_TO_RAD;
    float localX = dx * cos(thetaRad) + dy * sin(thetaRad);
    float localY = -dx * sin(thetaRad) + dy * cos(thetaRad);

    // Normierung für gleichmäßige Geschwindigkeit
    float norm = sqrt(localX * localX + localY * localY);
    if (norm > 0.0)
    {
        localX /= norm;
        localY /= norm;
    }

    move(localX, localY, 0.0); // Bewege Roboter
    return false; // Ziel noch nicht erreicht
}

// Sanfte asymmetrische Fahrt entlang der rechten Kante mit mehreren Sensoren
bool DriveSystem::moveAlongRightEdge()
{
    Serial.println("Starte asymmetrische sanfte Kantenfahrt...");

    setSpeedLimit(0.28); // Geschwindigkeit auf ca. 28% drosseln

    unsigned long startTime = millis();

    while (millis() - startTime < 30000) // max. 30 Sekunden fahren
    {
        // Sensoren rechts und vorne links abfragen
        bool fr = edgeSensors[1]->detectEdge(); // vorne rechts
        bool mr = edgeSensors[3]->detectEdge(); // mitte rechts
        bool br = edgeSensors[5]->detectEdge(); // hinten rechts
        bool fl = edgeSensors[0]->detectEdge(); // vorne links

        if (fl) // Ziel erreicht, vorne links sieht Kante
        {
            stop("fw");
            Serial.println("Ziel erreicht: vorne links sieht Kante.");
            return true;
        }

        float omega = 0.0; // Drehgeschwindigkeit
        float vy = 0.0;    // seitliche Geschwindigkeit

        // Verschiedene Fälle für Kantenlage
        if (fr && mr && br)
        {
            // Ganz rechts an der Kante → leicht links ausweichen
            vy = 0.3;
            omega = 0;
        }
        else if (fr && !mr && !br)
        {
            // Nur vorne rechts Kante → stark links drehen
            omega = 0.25;
            vy = 0.0;
        }
        else if (!fr && !mr && br)
        {
            // Nur hinten rechts Kante → leicht rechts drehen
            omega = -0.1;
            vy = 0.0;
        }
        else if (!fr && mr && !br)
        {
            // Nur Mitte rechts Kante → ideal
            omega = 0.0;
            vy = 0.0;
        }
        else if (!fr && !mr && !br)
        {
            // Keine Kante rechts → leicht links ausweichen
            omega = 0.0;
            vy = 0.05;
        }
        else
        {
            // Standardkorrektur, falls keine der Bedingungen passt
            omega = fr ? 0.15 : 0.0;
            omega -= br ? 0.05 : 0.0;
            vy = mr ? 0.05 : 0.0;
        }

        move(1.0, vy, omega); // Vorwärts mit Korrektur
        delay(20);
    }

    stop("fw"); // Stoppen nach Timeout
    return false; // Kante nicht erreicht
}

// Fahrt entlang der rechten Kante vorwärts für eine bestimmte Zeit
bool DriveSystem::moveAlongRightEdge(int time)
{
    setSpeedLimit(0.28);  // Geschwindigkeit begrenzen

    unsigned long startTime = millis();

    while (millis() - startTime < time)
    {
        bool fr = edgeSensors[1]->detectEdge(); // vorne rechts
        bool mr = edgeSensors[3]->detectEdge(); // mitte rechts
        bool br = edgeSensors[5]->detectEdge(); // hinten rechts


        float omega = 0.0;  // Drehgeschwindigkeit
        float vy = 0.0;     // seitliche Geschwindigkeit

        if (fr && mr && br)
        {
            // Komplett über Kante → deutlich nach links ausweichen
            vy = 0.3;
            omega = 0;
        }
        else if (fr && !mr && !br)
        {
            // Nur vorne rechts → stark nach links drehen
            omega = 0.25;
            vy = 0.0;
        }
        else if (!fr && !mr && br)
        {
            // Nur hinten rechts → leicht nach rechts drehen
            omega = -0.1;
            vy = 0.0;
        }
        else if (!fr && mr && !br)
        {
            // Nur Mitte sieht Kante → ideal
            omega = 0.0;
            vy = 0.0;
        }
        else if (!fr && !mr && !br)
        {
            // Alles sicher → leicht nach links ausweichen
            omega = 0.0;
            vy = 0.05;
        }
        else
        {
            // Standardkorrektur falls keine der Bedingungen passt
            omega = fr ? 0.15 : 0.0;
            omega -= br ? 0.05 : 0.0;
            vy = mr ? 0.05 : 0.0;
        }

        move(1.0, vy, omega); // Vorwärts mit Korrektur fahren
        delay(20);
    }

    stop("fw");  // Stoppen mit Bremsen vorwärts

    return true;
}

// Fahrt entlang der rechten Kante rückwärts, bis linke Kante erkannt wird (Abbruchbedingung)
bool DriveSystem::moveAlongRightEdgeBack()
{
    setSpeedLimit(0.28);
    unsigned long startTime = millis();

    while (millis() - startTime < 30000)
    {
        bool fr = edgeSensors[1]->detectEdge(); // vorne rechts (rückwärts hinten)
        bool mr = edgeSensors[3]->detectEdge(); // mitte rechts
        bool br = edgeSensors[5]->detectEdge(); // hinten rechts (rückwärts vorne)
        bool bl = edgeSensors[4]->detectEdge(); // hinten links (Abbruch)

        if (bl)
        {
            stop("bw");
            return true; // Abbruch bei linker Kante
        }

        float omega = 0.0;
        float vy = 0.0;

        if (fr && mr && br)
        {
            vy = 0.3;  // Deutlich nach links ausweichen
            omega = 0;
        }
        else if (fr && !mr && !br)
        {
            omega = 0.1;  // Stark links drehen
            vy = 0.0;
        }
        else if (!fr && !mr && br)
        {
            omega = -0.25;  // Leicht rechts drehen
            vy = 0.0;
        }
        else if (!fr && mr && !br)
        {
            omega = 0.0;   // Ideal mittig
            vy = 0.0;
        }
        else if (!fr && !mr && !br)
        {
            omega = 0.0;
            vy = 0.05;  // Leicht links ausweichen
        }
        else
        {
            omega = fr ? 0.15 : 0.0;
            omega -= br ? 0.05 : 0.0;
            vy = mr ? 0.05 : 0.0;
        }

        move(-1.0, vy, omega); // Rückwärts mit Korrektur fahren
        delay(20);
    }

    stop("bw");  // Stoppen mit Bremsen rückwärts

    return false; // Zeit abgelaufen ohne Abbruch
}

// Fahrt entlang der rechten Kante rückwärts für eine bestimmte Zeit mit Abbruch bei linker Kante
bool DriveSystem::moveAlongRightEdgeBack(int time)
{
    setSpeedLimit(0.28);
    unsigned long startTime = millis();

    while (millis() - startTime < time)
    {
        bool fr = edgeSensors[1]->detectEdge();
        bool mr = edgeSensors[3]->detectEdge();
        bool br = edgeSensors[5]->detectEdge();
        bool bl = edgeSensors[4]->detectEdge();

        if (bl)
        {
            stop("bw");
            return true;  // Linke Kante erkannt → abbrechen
        }

        float omega = 0.0;
        float vy = 0.0;

        if (fr && mr && br)
        {
            vy = 0.3;
            omega = 0;
        }
        else if (fr && !mr && !br)
        {
            omega = 0.1;
            vy = 0.0;
        }
        else if (!fr && !mr && br)
        {
            omega = -0.25;
            vy = 0.0;
        }
        else if (!fr && mr && !br)
        {
            omega = 0.0;
            vy = 0.0;
        }
        else if (!fr && !mr && !br)
        {
            omega = 0.0;
            vy = 0.05;
        }
        else
        {
            omega = fr ? 0.15 : 0.0;
            omega -= br ? 0.05 : 0.0;
            vy = mr ? 0.05 : 0.0;
        }

        move(-1.0, vy, omega);
        delay(20);
    }

    stop("bw");

    return true;
}

// Führt eine relative Drehung um targetAngle Grad durch, mit Timeout
bool DriveSystem::turn(float targetAngle)
{
    Serial.println("--- Starte einfache relative Drehung ---");

    float startAngle = gyro->getOrientation();
    float targetAbsAngle = startAngle + targetAngle - ANGLE_DIFF_BEFORE_STOP;

    // Zielwinkel in Bereich 0-360 Grad bringen
    if (targetAbsAngle >= 360.0)
        targetAbsAngle -= 360.0;
    if (targetAbsAngle < 0.0)
        targetAbsAngle += 360.0;

    unsigned long startTime = millis();
    const unsigned long timeout = 10000; // Timeout 10 Sekunden

    while (millis() - startTime < timeout)
    {
        float current = gyro->getOrientation();

        // Gegen Uhrzeigersinn drehen (links)
        if (startAngle > targetAbsAngle)
        {
            move(0.0, 0.0, 0.8);
            if (current <= targetAbsAngle)
            {
                stop();
                return true;
            }
        }
        // Im Uhrzeigersinn drehen (rechts)
        else
        {
            move(0.0, 0.0, -0.8);
            if (current >= targetAbsAngle)
            {
                stop();
                return true;
            }
        }

        delay(10);  // Kurzes Delay für Stabilität
    }

    stop();

    return false;  // Drehung nicht abgeschlossen (Timeout)
}
