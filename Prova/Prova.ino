#include <dht.h>
#include <LiquidCrystal.h>
#include <FirebaseESP32.h>
#include <WiFi.h>

const char WIFI_SSID[] = "Vivo-Internet-BF17";
const char WIFI_PASSWORD[] = "78814222";
const char FIREBASE_HOST[] = "https://prova-47a0e-default-rtdb.firebaseio.com/";
const char FIREBASE_AUTH[] = "wcilzTa3l5Q0APzA0wwtZ1bIY3zAIhPMnuek766L";
FirebaseData firebaseData;
FirebaseJson json;
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
dht DHT;
int pin[4] = {22, 21, 5, 2};
 
void setup() {
  Serial.begin(9600);
  pinMode(pin[0], OUTPUT); //22
  pinMode(pin[1], OUTPUT); //21
  pinMode(pin[2], OUTPUT); //5
  pinMode(pin[3], OUTPUT); //2
  lcd.begin(16, 2);
  DHT.read11(4);
  
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
  DHT.read11(4);
  if(DHT.temperature <= 25)
  {
    digitalWrite(pin[0], LOW);
    digitalWrite(pin[1], LOW);
    digitalWrite(pin[2], LOW);
    digitalWrite(pin[3], HIGH);
  }
  else if (DHT.temperature >= 27)
  {
    digitalWrite(pin[0], LOW);
    digitalWrite(pin[1], HIGH);
    digitalWrite(pin[2], LOW);
    digitalWrite(pin[3], LOW);
  }
  else
  {
    digitalWrite(pin[0], LOW);
    digitalWrite(pin[1], LOW);
    digitalWrite(pin[2], HIGH);
    digitalWrite(pin[3], LOW);
  }
    
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temperatura: ");
  lcd.print(DHT.temperature, 0);

  digitalWrite(pin[0], HIGH);
  json.set("/temperatura", DHT.temperature);
  json.set("/umidade", DHT.humidity);
  Firebase.updateNode(firebaseData, "/andre/Sensor", json);
  delay(1000);
  digitalWrite(pin[0], LOW);
  delay(29000);
}
