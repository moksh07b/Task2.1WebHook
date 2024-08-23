#include <WiFiNINA.h>   //Included The header File
#include "secrets.h"
#include "ThingSpeak.h" 

char ssid[] = SECRET_SSID;   // My SSID and Password is in the secret.h file
char pass[] = SECRET_PASS;   
WiFiClient  client;

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22     // Making the DHT object
DHT dht(DHTPIN, DHTTYPE);

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;


void setup() {
  Serial.begin(115200);       
  
  dht.begin(); // Beginning the reading of the DHT object
  ThingSpeak.begin(client);  // Initializing ThingSpeak 
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     // Delaying 5 seconds if it is not connected the first line 
    } 
    Serial.println("\nConnected.");
  }

  float h = dht.readHumidity();   //Humidity Variable
  float t = dht.readTemperature();  //Temperature Variable

  if (isnan(h) || isnan(t)) {
    // Serial.println(F("Failed to read from DHT sensor!"));   //If there is no data in the variables. 
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));    // For printing it on the serial monitor
  Serial.print(t);
  Serial.print(F("°C "));

  // set the fields with the values
  ThingSpeak.setField(1, t);    //Setting the field 1 for temperature
  ThingSpeak.setField(2, h);    //Setting the field 2 for humidity

  
  // writing to the ThingSpeak channel 
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // Getting the response back from connecting to the channel.
  
  if(x == 200){   // 200 is the "OK" response.
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    
  delay(60000); // Wait 60 seconds to update the channel again
}
