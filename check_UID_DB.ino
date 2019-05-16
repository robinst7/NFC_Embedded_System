#include <SPI.h>
#include <Ethernet.h>
#include <PN532_I2C.h>
#include <NfcAdapter.h>
#include <Wire.h>
#include <PN532.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
 
IPAddress ip(192,168,1,187); //IP address for your arduino.

char server[] = "192.168.1.86"; //IP address of your computer.


String output = "";

String rcv=""; //Variable in which the server response is recorded.

EthernetClient client;

void setup()
{
  
  Serial.begin(9600);
  nfc.begin();
  Ethernet.begin(mac, ip);
  
  delay(5000); //Wait for ethernet to connect.

}

void httpRequest()
{
  output = "";
  if (client.connect(server, 80)) 
  {
    Serial.println("Connection established 1");
    client.print(String("GET ") + "/final/tryjson.php/" + " HTTP/1.1\r\n" + "Host: " + server + "\r\n" + "Connection: close\r\n\r\n"); //GET request for server response.
    unsigned long timeout = millis();
    while (client.available() == 0) 
    {
      if (millis() - timeout > 25000) //If nothing is available on server for 25 seconds, close the connection.
      { 
        return;
      }
    }
    while(client.available())
    {
      String line = client.readStringUntil('\r'); //Read the server response line by line..
      rcv+=line; //And store it in rcv.
     
    }
    client.stop(); // Close the connection.
  }
  else
  {
    Serial.println("Connection failed 1");
  }
  String s2=rcv.substring((rcv.indexOf('[')),rcv.indexOf(']')); // Extract the line returned by JSON object.
 
  output = s2;

  
}

void loop() 
{
 
  

    Serial.println("\nScan Your ID Card");
    if(nfc.tagPresent())
{
     NfcTag tag = nfc.read();
     String uid = tag.getUidString();
     uid.replace(" ","");
      httpRequest(); //Call the function to read the response from the server.
      Serial.println(output);
      
      if(output.indexOf(uid) > 0) {
        Serial.println("Successful");
      }
      else{
        Serial.println("Unsuccessful");
      }
      delay(1000);
}   
  
  delay(10000);
}
