#include <SPI.h>
#include <Ethernet.h>
#include <PN532_I2C.h>
#include <NfcAdapter.h>
#include <Wire.h>
#include <PN532.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
byte mac[] = { 0xEE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
IPAddress ip(192,168,1,187); //IP address for your arduino.

char server[] = "192.168.1.86"; //IP address of your computer.




EthernetClient client;

void setup()
{
  
  Serial.begin(9600);

  nfc.begin();
 
  Ethernet.begin(mac, ip);
  
  delay(5000); //Wait for ethernet to connect.

}

void loop() 
{
 
   Serial.println("\nScan your ID Card to add it to the system");

   if(nfc.tagPresent())
   {
    
       NfcTag tag = nfc.read();
       String uid = tag.getUidString();
       uid.replace(" ","");
      if (client.connect(server, 80)) 
      {
      Serial.println("Connection Established to DB");
      client.print("GET /final/cards.php?"); //GET request to write data to the database.
      client.print("uid=");
      client.print(uid);
      client.println(" HTTP/1.1"); 
      client.println("Host: 192.168.137.1"); 
      client.println("Connection: close"); 
      client.println(); 
      client.println(); 
      client.stop();
      }
      else
      {
        Serial.println("Connection failed 2");
      }
  
   }
  delay(10000);
}
