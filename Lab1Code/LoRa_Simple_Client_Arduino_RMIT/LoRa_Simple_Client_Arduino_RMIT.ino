#include <SPI.h>
#include <RH_RF95.h>


RH_RF95 rf95; // Create radio object
float frequency = 916.00; // This frequency is for Group 1, you will need to change your frequency according to the provided table
void setup() {
  Serial.begin(9600);
  Serial.println("Start LoRa Client");
  if (!rf95.init())
    Serial.println("init failed");
  
  rf95.setFrequency(frequency); 
  
  // Setup Power,dBm
  rf95.setTxPower(3); // Do not increase the transmit power as you do not need to operate over a long range
  
  rf95.setSpreadingFactor(7); // Setup Spreading Factor (6 ~ 12)
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  //Lower BandWidth for longer distance.
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);
}

void loop() {
  Serial.println("Sending message to LoRa Server");       //Debug output to serial monitor
  // Send a message to LoRa Server
  uint8_t data[] = "Hello, This is RMIT Lab Group 1";     //Data goes here - String by default
  rf95.send(data, sizeof(data));                          //Send data via the radio module
  
  rf95.waitPacketSent();                                  //Wait for the Tx to complete
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];                   //Buffer for storing the response data 
  uint8_t len = sizeof(buf);                              //Amount of data sent back

  if (rf95.waitAvailableTimeout(3000)) {                  //Wait 3 sec for data to be available 
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len)) {                           //Read the recv data into buffer
      
      Serial.print("got a reply: ");                      //Provide debug output to serial monitor
      Serial.println((char*)buf);
      Serial.print("RSSI: ");                             //Include Received Signal Strength
      Serial.println(rf95.lastRssi(), DEC);    
    } else {
      Serial.println("recv failed");                      //Something went wrong - no data Rx'd
    }
  } else {
    Serial.println("No reply, is LoRa server running?");  //Timed out - let the user know
  }
  delay(5000);
}
