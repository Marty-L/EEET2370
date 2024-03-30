#include <dht.h>      //Using library from http://arduino.cc/playground/Main/DHTLib

dht DHT;    

//GLOBAL VARIABLES
#define DHT11_PIN 5

void setup() {
  Serial.begin(9600);                 //Set up serial port for logging output.
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
  delay(2000);
}
