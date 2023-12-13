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

#define led0 2
#define led1 3
#define led2 6
#define led3 7


#define ist 8
#define botaoPin 9

// Inicializa o sensor nos pino 4 e 5 
Ultrasonic ultrasonic(triggerPin, echoPin);
//ModbusIP object
ModbusIP mb;
// objeto DHT11
DHT dht(DHTPIN, DHTTYPE);

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

    mb.addIreg(0);
    mb.addIreg(1);
    mb.addIreg(2);
    mb.addIreg(3);
    mb.addIreg(4);

    mb.addCoil(0);
    mb.addCoil(1);
    mb.addCoil(2);
    mb.addCoil(3);
    

    mb.addIsts(0);

}

void loop() {
   //Call once inside loop() - all magic here
   mb.task();
   
   long microsec = ultrasonic.timing();

   float umidadeAr = dht.readHumidity();                            //IREG
   float temperaturaAr = dht.readTemperature();                    //IREG
   int luminosidade = analogRead(ldrPin);                         //IREG
   int distancia = ultrasonic.convert(microsec, Ultrasonic::CM); //IREG
   umidade  = analogRead(umidadePin);                           //IREG

   mb.Ireg(0, umidadeAr);
   mb.Ireg(1, temperaturaAr);
   mb.Ireg(2, luminosidade);
   mb.Ireg(3, distancia);
   mb.Ireg(4, umidade);

   mb.Ists(0, digitalRead(botaoPin));

   digitalWrite(led0, mb.Coil(0));
   digitalWrite(led1, mb.Coil(1));
   digitalWrite(led2, mb.Coil(2));
   digitalWrite(led3, mb.Coil(3));
   

}
