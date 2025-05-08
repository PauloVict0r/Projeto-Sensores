#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  27   // SDA
#define RST_PIN 26  // RST

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();          // Inicia SPI
  mfrc522.PCD_Init();   // Inicia MFRC522
  Serial.println("Aproxime o cartão RFID...");
}

void loop() {
  // Verifica se há um novo cartão
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Exibe o UID do cartão
  Serial.print("UID do cartão: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  // Finaliza comunicação
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}
