/*
  Modbus-Arduino Example - TempSensor (Modbus IP)
  Copyright by André Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino

 
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
#include <Ultrasonic.h>
#include "DHT.h"

#define triggerPin 4
#define echoPin 5

#define umidadePin A0
int umidade;

#define DHTPIN A1
#define DHTTYPE DHT11

#define ldrPin A2

// Inicializa o sensor nos pino 4 e 5 
Ultrasonic ultrasonic(triggerPin, echoPin);
//ModbusIP object
ModbusIP mb;
// objeto DHT11
DHT dht(DHTPIN, DHTYPE);

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // The IP address for the shield
    byte ip[] = { 192, 168, 0, 120 };   
    //Config Modbus IP 
    mb.config(mac, ip);
    dht.begin(); // inicializa o sensor DHT11
    
    Serial.begin(9600);

    pinMode(umidadePin, INPUT); // define pino do sensor de umidade do solo como INPUT
    pinMode(ldrPin, INPUT);

}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   long microsec = ultrasonic.timing();

   float umidadeAr = dht.readHumidity();
   float temperaturaAr = dht.readTemperature();
   int luminosidade = analogRead(ldrPin);
   distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
   umidade  = analogRead(umidadePin);
}
*/
#include <SPI.h>
#include <Ethernet.h>
#include <Modbus.h>
#include <ModbusIP.h>
#include <Ultrasonic.h>
//#include "DHT.h"

#define triggerPin 4
#define echoPin 5

#define umidadePin A0
int umidadeSolo;
int distancia;

#define DHTPIN A1
#define DHTTYPE DHT11

#define ldrPin A2

// Inicializa o sensor nos pino 4 e 5 
Ultrasonic ultrasonic(triggerPin, echoPin);
//ModbusIP object
ModbusIP mb;
// objeto DHT11
//DHT dht(DHTPIN, DHTTYPE);

void setup() {
    // The media access control (ethernet hardware) address for the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
    // The IP address for the shield
    byte ip[] = { 192, 168, 0, 120 };   
    //Config Modbus IP 
    mb.config(mac, ip);
//    dht.begin(); // inicializa o sensor DHT11
    
    Serial.begin(9600);

    pinMode(umidadePin, INPUT); // define pino do sensor de umidade do solo como INPUT
    pinMode(ldrPin, INPUT);

    // adicionar 5 Ireg
    for (int i=0; i<=4; i++){
      mb.addIreg(i);
        
    }
    

}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   long microsec = ultrasonic.timing();

//   float umidadeAr = dht.readHumidity();
//   float temperaturaAr = dht.readTemperature();
   int luminosidade = analogRead(ldrPin);
   distancia = ultrasonic.convert(microsec, Ultrasonic::CM);
   umidadeSolo = analogRead(umidadePin);

//   mb.Ireg(0, umidadeAr);
//  mb.Ireg(1, temperaturaAr);
   mb.Ireg(2, luminosidade);
   mb.Ireg(3, distancia);
   mb.Ireg(4, umidadeSolo);
}
