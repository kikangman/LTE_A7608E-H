#include <Arduino.h>

// ESP32 pin mapping for SIMCom A7608E-H
static constexpr int MODEM_TX_PIN = 8;   // ESP32 -> Modem RX
static constexpr int MODEM_RX_PIN = 9;   // ESP32 <- Modem TX
static constexpr int MODEM_PWRKEY_PIN = 10; // Modem PWRKEY (K)

HardwareSerial &MODEM = Serial1;

static void powerOnModem() {
  pinMode(MODEM_PWRKEY_PIN, OUTPUT);
  digitalWrite(MODEM_PWRKEY_PIN, HIGH);
  delay(20);
  digitalWrite(MODEM_PWRKEY_PIN, LOW);
  delay(1100); // Minimum pulse to turn on
  digitalWrite(MODEM_PWRKEY_PIN, HIGH);
  delay(3000); // Shortened boot wait
}

void setup() {
  Serial.begin(115200);
  delay(200);

  powerOnModem();

  MODEM.begin(115200, SERIAL_8N1, MODEM_RX_PIN, MODEM_TX_PIN);
  Serial.println("A7608E-H ready: TX=8 RX=9 PWRKEY=10");
  Serial.println("Type AT commands below.\n");
}

void loop() {
  // Bridge between USB Serial and modem UART for quick testing
  while (Serial.available()) {
    MODEM.write(Serial.read());
  }
  while (MODEM.available()) {
    Serial.write(MODEM.read());
  }
}