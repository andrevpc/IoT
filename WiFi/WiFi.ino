#include <FirebaseESP32.h>
#include <WiFi.h>
#include <dht.h>
#include <LiquidCrystal.h> //https://www.circuitschools.com/interfacing-16x2-lcd-module-with-esp32-with-and-without-i2c/

const char WIFI_SSID[] = "Vivo-Internet-BF17"; //Vivo-Internet-BF17
const char WIFI_PASSWORD[] = "78814222"; //78814222
const char FIREBASE_HOST[] = "https://teste-1cdf0-default-rtdb.firebaseio.com/"; // URL acima do console de dados
const char FIREBASE_AUTH[] = "Qh4s7DBwnmBliJwqO07fysRVkl4x8ukC3TUdikpG"; // Configurações do projeto > Contas de serviço > Chaves secretas do banco de dados > Copiar chave secreta
FirebaseData firebaseData;
FirebaseJson json;
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
dht DHT;

void setup() { 
  Serial.begin(115200);
  
  lcd.begin(16, 2);
  DHT.read11(22);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");
}

void loop() {
  DHT.read11(22);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temperatura: ");
  lcd.print(DHT.temperature, 0);
  json.set("/temperatura", DHT.temperature);
  json.set("/umidade", DHT.humidity);
  Firebase.updateNode(firebaseData, "/andre/Sensor", json);
  delay(1000);
}
