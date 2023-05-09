#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "DHT.h"

BLECharacteristic *characteristicTX; // através desse objeto iremos enviardados para o client
bool deviceConnected = false;        // controle de dispositivo conectado

const int LED = 13; 

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "ab8028b1-198e-4351-b779-901fa0e0371e" // UART service UUID
#define CHARACTERISTIC_UUID_RX "4aca9682-9736-4e5d-932b-e9b31405049c"
#define CHARACTERISTIC_UUID_TX "0972128C-7613-4075-AD52-756F33D4DA91"
#define DHTPIN 27
// our sensor is DHT11 type
#define DHTTYPE DHT11
// create an instance of DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// callback para receber os eventos de conexão de dispositivos
class ServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer *pServer)
  {
    deviceConnected = false;
  }
};

// callback para envio das características
class CharacteristicCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic)
  {
    // retorna ponteiro para o registrador contendo o valor atual dacaracteristica

    std::string rxValue = characteristic->getValue();
    // verifica se existe dados (tamanho maior que zero)
    if (rxValue.length() > 0)
    {
      Serial.println("*********");
      Serial.print("Received Value: ");

      for (int i = 0; i < rxValue.length(); i++)
      {
        Serial.print(rxValue[i]);
      }
      Serial.println();
      // Realiza um comando de acordo com a mensagem recebida
      if (rxValue.find("L1") != -1)
      {
        Serial.print("Turning LED ON!");
        digitalWrite(LED, HIGH);
      }
      else if (rxValue.find("L0") != -1)
      {
        Serial.print("Turning LED OFF!");
        digitalWrite(LED, LOW);
      }
      Serial.println();
      Serial.println("*********");
    }
  }
};

void setup()
{
  Serial.begin(115200);

  dht.begin();

  pinMode(LED, OUTPUT);

  // Create the BLE Device
  BLEDevice::init("Nyck-ESP"); // nome do dispositivo bluetooth
  // Cria o BLE Server
  BLEServer *server = BLEDevice::createServer(); // cria um BLE server
  server->setCallbacks(new ServerCallbacks());   // seta o callback do server
  // Cria o BLE Service
  BLEService *service = server->createService(SERVICE_UUID);
  // Cria uma BLE Characteristic para envio de dados
  characteristicTX = service->createCharacteristic(
      CHARACTERISTIC_UUID_TX,
      BLECharacteristic::PROPERTY_NOTIFY);
  characteristicTX->addDescriptor(new BLE2902());
  // Cria uma BLE Characteristic para recebimento de dados
  BLECharacteristic *characteristic = service->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE);

  characteristic->setCallbacks(new CharacteristicCallbacks());
  // Start the service
  service->start();
  // Start advertising (ESP fica visível para outros dispositivos)
  server->getAdvertising()->start();

  Serial.println("Waiting a client connection to notify...");
}

void loop()
{
  // se existe algum dispositivo conectado
  if (deviceConnected)
  {
    float t = dht.readTemperature();

    // Convertendo para char para poder enviar pelo BLE:
    char txString[8];           // garantindo que é grande o suficiente
    dtostrf(t, 1, 2, txString); // float_val, min_width, digits_after_decimal, char_buffer

    characteristicTX->setValue(txString); // seta o valor que acaracteristica notificará (enviar)

    characteristicTX->notify(); // Envia o valor para o cliente(smartphone)

    Serial.print("*** Sent Value: ");
    Serial.print(txString);
    Serial.println(" ***");
  }
  delay(1000);
}
