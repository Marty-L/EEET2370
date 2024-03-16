#define BAUDRATE 115200

#include <Console.h>
#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 rf95;

int led = A2;                               //LED for visual feedback of data being Rx'd
float frequency = 916.0;                    //Group 1 RF freq. (916.0 MHz default)

void setup() {
  pinMode(led, OUTPUT);     
  Bridge.begin(BAUDRATE);
  Console.begin();
  while (!Console) ; // Wait for console port to be available
  Console.println("Start Sketch");
  if (!rf95.init())
    Console.println("init failed");

  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(3);
  
  // Setup Spreading Factor (6 ~ 12)
  rf95.setSpreadingFactor(7);
  
  // Setup BandWidth, option: 7800,10400,15600,20800,31200,41700,62500,125000,250000,500000
  rf95.setSignalBandwidth(125000);
  
  // Setup Coding Rate:5(4/5),6(4/6),7(4/7),8(4/8) 
  rf95.setCodingRate4(5);
  
  Console.print("Listening on frequency: ");            //Serial console debug - echo freq.
  Console.println(frequency);
}

void loop() {
  if (rf95.available()) {                               //Check if RF module in use
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];               //Buffer for reading Rx'd data into
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)) {                           //Have Rx'd data from RF module
      digitalWrite(led, HIGH);                            //Visual feedback via LED - data Rx'd
      RH_RF95::printBuffer("request: ", buf, len);        //Write Rx'd data to buffer
      uint8_t groupID = buf[0];                           //Strip the header from the data
      
      Console.print("got request: ");                     //Echo back to user via serial console
      Console.print("Group ");
      Console.write(groupID);
      Console.print(" says:"); 
      Console.println((char*)buf[1]);                     //Print the Rx'd data
      Console.print("RSSI: ");                            //Include Received Signal Strength
      Console.println(rf95.lastRssi(), DEC);
      
      // Send a reply
      String message = "And hello back to you Group " + String(groupID);  //Create the message (with groupID).
      uint8_t data[RH_RF95_MAX_MESSAGE_LEN];                              //Reply data goes here
      message.getBytes(data, message.length() + 1);                       //Copy the message bytes to reply data...
                                                                          //...+ 1 for null byte
      rf95.send(data, sizeof(data));                                      //Send response via RF module
      rf95.waitPacketSent();                                              //Wait for Tx flag to clear
      Console.println("Sent a reply");                                    //...let the user know
      digitalWrite(led, LOW);
      
    } else {
      Console.println("recv failed");                   //Whoops! Something went wrong...
                                                        //...better tell somebody.
    }
  }
}
