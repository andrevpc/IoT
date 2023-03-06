#include <dht.h>
#include <LiquidCrystal.h>


// //Contador Senai
// const int buttonPin = 11;
// const int buttonPinDecrement = 12;
// int inputs[] = {2, 3, 4, 5, 6, 7, 8, 9};

// int numPin[11][7] =
//     {
//         {0, 0, 0, 0, 0, 0, 1}, // 0
//         {1, 0, 0, 1, 1, 1, 1}, // 1
//         {0, 0, 1, 0, 0, 1, 0}, // 2
//         {0, 0, 0, 0, 1, 1, 0}, // 3
//         {1, 0, 0, 1, 1, 0, 0}, // 4
//         {0, 1, 0, 0, 1, 0, 0}, // 5
//         {0, 1, 0, 0, 0, 0, 0}, // 6
//         {0, 0, 0, 1, 1, 1, 1}, // 7
//         {0, 0, 0, 0, 0, 0, 0}, // 8
//         {0, 0, 0, 0, 1, 0, 0}, // 9,
// };

// void setup()
// {
//   for (int i = 0; i < sizeof(inputs); i++)
//   {
//     pinMode(inputs[i], OUTPUT);
//   }
//   pinMode(10, OUTPUT);
  
//   Serial.begin(9600);
//   pinMode(buttonPin, INPUT_PULLUP);
//   pinMode(buttonPinDecrement, INPUT_PULLUP);
  
// }

// int actualNum = 0;
// int on = 0;
// int buttonState = 0;
// int buttonStateDecrement = 0;

// void loop()
// {
//   buttonState = digitalRead(buttonPin);

//   if(buttonState == 1) 

//   {
//     if(actualNum < 9)
//     {
//      actualNum++; 
//     }
//     else
//     {
//       actualNum = 0;
//     }
//     delay(250);
//   }

//   buttonStateDecrement = digitalRead(buttonPinDecrement);

//   if(buttonStateDecrement == 1) 
//   {
//     if(actualNum > 0)
//     {
//      actualNum--; 
//     }
//     else
//     {
//       actualNum = 9;
//     }
//     delay(250);
//   }
//   for (int i = 0; i < sizeof(inputs); i++)
//   {
//     digitalWrite(inputs[i], numPin[actualNum][i]);
//   }
//   Serial.println(buttonStateDecrement);
// }

// //Blink

// // the setup function runs once when you press reset or power the board
// void setup() {
//   // initialize digital pin LED_BUILTIN as an output.
//   pinMode(2, OUTPUT);
// }

// // the loop function runs over and over again forever
// void loop() {
//   digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
//   delay(1000);                       // wait for a second
//   digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
//   delay(1000);                       // wait for a second
// }

// bool in;

//  void setup(){
//  pinMode(32,INPUT);
//  pinMode(2,OUTPUT);
// }
// void loop(){
//  in = digitalRead(32);
//  digitalWrite(2,in);
// }

// // Luzes piscando com diminuição do delay pelo potenciometro e botao para inverter o sentido
// int pin[4] = {13, 12, 14, 27};
// int potenciometroPin = 34;
// int i = 0;
// int buttonPin = 33;
// int dht11Pin = 32;
// dht DHT;
// int botao = 0;
// int botaoState = 1; //1 = mais ; 0 = menos

// void setup() {
//   Serial.begin(9600);
//   pinMode(pin[0], OUTPUT); //13
//   pinMode(pin[1], OUTPUT); //12
//   pinMode(pin[2], OUTPUT); //14
//   pinMode(pin[3], OUTPUT); //27
//   pinMode(potenciometroPin, INPUT); // potenciometro
//   pinMode(buttonPin, INPUT_PULLUP); // botão
// }

// void loop() {
//   while (i < 4)
//   {
//     DHT.read11(dht11Pin);
//     Serial.print("Umidade: ");
//     Serial.println(DHT.humidity);
//     Serial.print("Temperatura:");
//     Serial.println(DHT.temperature, 0);
    
//     botao = digitalRead(buttonPin);
//     Serial.println(analogRead(25)/8+25);
//     if(botao == 1)
//     {
//       botaoState = (botaoState == 0) ? 1 : 0;
//       delay(250);
//     }
//     if(botaoState == 1)
//     {
//       digitalWrite(i != 0 ? pin[i-1] : pin[3], LOW);
//       digitalWrite(i != 3 ? pin[i+1] : pin[0], LOW);
//       digitalWrite(pin[i], HIGH);
//       delay(analogRead(25)/8+25);
//       i++;
//     }
//     else
//     {
//       digitalWrite(i != 3 ? pin[i+1] : pin[0], LOW);
//       digitalWrite(i != 0 ? pin[i-1] : pin[3], LOW);
//       digitalWrite(pin[i], HIGH);
//       delay(analogRead(25)/8+25);
//       i--;
//       i = (i == -1) ? 3 : i;
//     }
//   }
//   i = (botaoState == 1) ? 0 : 3;
// }

// // LCD com umidade e temperatura
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);
dht DHT;
int hum = 0;
int tem = 0;
 
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  DHT.read11(22);
  hum = DHT.humidity;
  tem = DHT.temperature;
}
 
void loop() {
  DHT.read11(22);
  if(hum != DHT.humidity || tem != DHT.temperature)
  {
    lcd.clear();
  }
  lcd.setCursor(0, 0);
  lcd.print("Umidade: ");
  lcd.print(DHT.humidity);
  lcd.setCursor(0, 1);
  lcd.print("Temperatura: ");
  lcd.print(DHT.temperature, 0);
}
