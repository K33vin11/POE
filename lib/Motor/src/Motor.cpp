#include <Motor.h>
#include <Arduino.h>

// Konstruktor: Initialisiert die Motor-Instanz mit der I2C-Adresse und ob die Drehrichtung invertiert ist
Motor::Motor(uint8_t address, bool invertDirection)
    : i2cAddress(address), invert(invertDirection) {}

// Setzt die Motorgeschwindigkeit (speed: -255 bis 255)
// Negativer Wert = Rückwärts, positiver Wert = Vorwärts
void Motor::setSpeed(int speed)
{
    // Wenn invert true ist, wird das Vorzeichen von speed umgekehrt
    if (invert)
        speed = -speed;

    uint8_t direction;

    // Geschwindigkeit 0 bedeutet stoppen
    if (speed == 0)
    {
        direction = 0;      // Richtung 0 = Stopp
        sendSpeed(direction, 0); // Geschwindigkeit 0 senden
        return;
    }
    else if (speed > 0)
    {
        direction = 1;      // Richtung 1 = vorwärts
    }
    else
    {
        direction = 2;      // Richtung 2 = rückwärts
        speed = -speed;     // Geschwindigkeit positiv machen für PWM
    }

    // Kickstart: Für kurzen Moment die doppelte Geschwindigkeit senden,
    // um den Motor "anzutreiben" (max. 255)
    int boostedSpeed = speed * 2;
    if (boostedSpeed > 255)
        boostedSpeed = 255;

    sendSpeed(direction, boostedSpeed);  // Gespeedete Geschwindigkeit senden
    delay(30);                           // 30 ms Pause, um Kickstart wirken zu lassen
    sendSpeed(direction, speed);         // Dann normale Geschwindigkeit senden
}

// Sendet die Steuerdaten über I2C an den Motorcontroller
// direction: Richtungscode (0=stop,1=vorwärts,2=rückwärts)
// speed: PWM-Wert 0-255
void Motor::sendSpeed(uint8_t direction, int speed)
{
    uint16_t pwm16 = speed * 255;   // 16-Bit PWM-Wert (vermutlich fehlerhaft, aber so vom Original)
    uint16_t freq = 1000;           // PWM-Frequenz (1000 Hz)

    Wire.beginTransmission(i2cAddress);
    Wire.write(0x00);           // Befehl: Motorsteuerung
    Wire.write(direction);      // Richtung senden
    Wire.write(speed);          // Geschwindigkeit (8-bit) senden
    Wire.write(pwm16 & 0xFF);  // LSB von 16-Bit PWM
    Wire.write(pwm16 >> 8);    // MSB von 16-Bit PWM
    Wire.write(freq & 0xFF);   // LSB der Frequenz
    Wire.write(freq >> 8);     // MSB der Frequenz
    Wire.endTransmission();
}

void Motor::begin()
{
    Serial.println("Motor initialisieren...");
    Serial.print("Motor initialisiert");
}

// Motor anhalten mit optionaler Richtungssimulation ("fw" für Vorwärtsbremsung, "bw" für Rückwärtsbremsung)
void Motor::stop(String direction)
{
    if (direction == "fw")
    {
        setSpeed(-100);    // Kurz rückwärts anbremsen
        delay(80);
        setSpeed(0);       // Motor stoppen
    }
    else if (direction == "bw") {
        setSpeed(100);     // Kurz vorwärts anbremsen
        delay(80);
        setSpeed(0);       // Motor stoppen
    }
}

// Motor sofort stoppen (Speed 0 senden)
void Motor::stop()
{
    setSpeed(0); // Stoppen = Speed 0 senden
}
