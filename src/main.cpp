#include <WiFi.h>
#include <PubSubClient.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN  27   // SDA
#define RST_PIN 26   // RST

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER;

WiFiClient espClient;
PubSubClient client(espClient);
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  delay(5000);

  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  // Conectar ao Wi-Fi
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi");

  /*

  -- mqtt
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Configurar o servidor MQTT
  client.setServer(mqtt_server, 1883);
  */

  }

void reconnect();

void loop() {
  /*
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  */

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uidString += " ";
  }

  Serial.print("UID do cartão: ");
  Serial.println(uidString);

  // Publicar o UID no tópico "rfid/uid"
  client.publish("rfid/uid", uidString.c_str());

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(1000);
}

void reconnect() {
  // Loop até reconectar
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}
