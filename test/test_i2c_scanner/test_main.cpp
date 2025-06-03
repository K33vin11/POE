#include <Arduino.h>
#include <Wire.h>
#include <unity.h>
#include <Config.h>

// Liste der erwarteten I2C-Adressen im Projekt
const uint8_t expectedAddresses[] = {
  MOTOR1_I2C_ADDRESS,
  MOTOR2_I2C_ADDRESS,
  MOTOR3_I2C_ADDRESS,
  MOTOR4_I2C_ADDRESS
};

bool addressFound(uint8_t addr, const uint8_t* foundList, size_t foundCount) {
  for (size_t i = 0; i < foundCount; ++i) {
    if (foundList[i] == addr) return true;
  }
  return false;
}

void test_i2c_addresses_detected() {
  uint8_t foundAddresses[16]; // Speicher für gefundene Adressen
  size_t foundCount = 0;

  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      foundAddresses[foundCount++] = address;
      Serial.print(" Gefunden: 0x");
      Serial.println(address, HEX);
    }
  }

  // Teste, ob alle erwarteten Adressen gefunden wurden
  for (size_t i = 0; i < sizeof(expectedAddresses); ++i) {
    char message[64];
    sprintf(message, "I2C-Adresse 0x%02X wurde nicht gefunden!", expectedAddresses[i]);
    TEST_ASSERT_TRUE_MESSAGE(addressFound(expectedAddresses[i], foundAddresses, foundCount), message);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  delay(2000); // Warten auf Serial

  UNITY_BEGIN();
  RUN_TEST(test_i2c_addresses_detected);
  UNITY_END();
}

void loop() {
  // nichts tun
}
