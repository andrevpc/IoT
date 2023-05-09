#include <Wire.h>
#include <VL53L0X.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Servo.h>

const char* ssid = "Murilindo";
const char* password = "andregato";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String caminho = "ok";

const char* PARAM_INPUT_1 = "slider";
const char* PARAM_INPUT_2 = "value";

static const int servoPin1 = 4; //Radar
bool scanon = false;

int servo1;
int servo2;
int servo3;
int servo4;

int IN1 = 5;
int IN2 = 18;
int IN3 = 19;
int IN4 = 23;

int servopos1;
int servopos2;
int servopos3;
int servopos4;

int tempo = 20;

VL53L0X sensor;
Servo radarBraco;

Servo Base;
Servo Cutuvelo;
Servo Pulso;
Servo Garra;

unsigned long now = 0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Base.attach(27);
  Cutuvelo.attach(26);
  Pulso.attach(25);
  Garra.attach(33);

  Wire.begin();
  
  radarBraco.attach(servoPin1);
  sensor.init();
  sensor.setTimeout(500);
  
  Serial.begin(115200);
  Serial.println();

  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);



  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/FO", HTTP_GET, [](AsyncWebServerRequest * request) {
    UP();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/FF", HTTP_GET, [](AsyncWebServerRequest * request) {
    STOP();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/TO", HTTP_GET, [](AsyncWebServerRequest * request) {
    DOWN();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/TF", HTTP_GET, [](AsyncWebServerRequest * request) {
    STOP();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/DO", HTTP_GET, [](AsyncWebServerRequest * request) {
    RIGHT();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/DF", HTTP_GET, [](AsyncWebServerRequest * request) {
    STOP();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/EO", HTTP_GET, [](AsyncWebServerRequest * request) {
    LEFT();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/EF", HTTP_GET, [](AsyncWebServerRequest * request) {
    STOP();
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/SCANON", HTTP_GET, [](AsyncWebServerRequest * request) {
    scanon = true;
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/SCANOFF", HTTP_GET, [](AsyncWebServerRequest * request) {
    scanon = false;
    request->send_P(200, "text/plain", caminho.c_str());
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest * request) {
    String inputMessage1;
    String inputMessage2;
    Serial.println(inputMessage2);
    // GET input1 value on <ESP_IP>/update?slider=<inputMessage1>&value=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      switch (inputMessage1.toInt()) {
        case 1:
          servo1 = inputMessage2.toInt();
          break;
        case 2:
          servo2 = inputMessage2.toInt();
          break;
        case 3:
          servo3 = inputMessage2.toInt();
          break;
        case 4:
          servo4 = inputMessage2.toInt();
          break;
        default:
          break;
      }
      Serial.println(inputMessage2);
    }

    request->send(200, "text/plain", inputMessage1 + ";" +  inputMessage2);
  });

  // Start server
  server.begin();
}

void loop() {
  if (!(millis() < now + tempo))
  {
    now = millis();
    moveServos();
    if (scanon)
    {
      scan();
    }
  }
}

void moveServos()
{
  if (servo1 > servopos1)
  {
    servopos1++;
    Base.write(servopos1);
  }
  else if (servo1 < servopos1)
  {
    servopos1--;
    Base.write(servopos1);
  }

  if (servo2 > servopos2)
  {
    servopos2++;
    Cutuvelo.write(servopos2);
  }
  else if (servo2 < servopos2)
  {
    servopos2--;
    Cutuvelo.write(servopos2);
  }

  if (servo3 > servopos3)
  {
    servopos3++;
    Pulso.write(servopos3);
  }
  else if (servo3 < servopos3)
  {
    servopos3--;
    Pulso.write(servopos3);
  }

  if (servo4 > servopos4)
  {
    servopos4++;
    Garra.write(servopos4);
  }
  else if (servo4 < servopos4)
  {
    servopos4--;
    Garra.write(servopos4);
  }
}

void scan()
{
  for(int posDegrees = 180; posDegrees >= 0; posDegrees=posDegrees-1) {
    radarBraco.write(posDegrees);
    int dist = sensor.readRangeSingleMillimeters();
    Serial.print(dist);
    Serial.print(" ");
    Serial.print(posDegrees);
    Serial.print("\n");
    delay(17);
  }
  for(int posDegrees = 1; posDegrees <= 180; posDegrees=posDegrees+1) {
    radarBraco.write(posDegrees);
    int dist = sensor.readRangeSingleMillimeters();
    
    Serial.print(dist);
    Serial.print(" ");
    Serial.print(posDegrees);
    Serial.print("\n");
    delay(17);
  }
}

void STOP()
{
  digitalWrite(IN1, LOW); //RIGHT
  digitalWrite(IN2, LOW); //RIGHT
  digitalWrite(IN3, LOW); //LEFT
  digitalWrite(IN4, LOW); //LEFT
}

void UP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void DOWN()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void RIGHT()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void LEFT()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
