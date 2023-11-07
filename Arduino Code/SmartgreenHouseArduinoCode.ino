#include "DHT.h"
#include<Adafruit_Sensor.h>
#include <Wire.h>



#define DHTTYPE DHT11
#define DHTPIN 14 //D5 pin= GPIO pin 14
int soilPin = A0;
int buzzer0  = 5;

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

float temperature, humidity,soilmoi;

DHT dht(DHTPIN, DHTTYPE);; //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "No ID"
#define WIFI_PASSWORD "chipi2020"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCNrCCFMvY1YJPkcD2OeV8J1RGmp8muhCM"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-green-house-7cde7-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

//unsigned long sendDataPrevMillis = 0;
//int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  pinMode(0, OUTPUT);  // Ensure the pin is configured as an OUTPUT
  digitalWrite(0, HIGH);

  pinMode(16, OUTPUT);  // Ensure the pin is configured as an OUTPUT
  digitalWrite(16, HIGH);
  
  

  pinMode(10, INPUT);
  pinMode(buzzer0, OUTPUT);
 
  pinMode(13,OUTPUT);

  dht.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);



}


void loop(){
  
   Serial.println("Loop started.");
// Turn the relay ON
 
  

 delay(1000);
  float h = dht.readHumidity();
   
  float t = dht.readTemperature();
  int soilMoisture = analogRead(soilPin);  // Read the analog value from the sensor
  int moisturePercentage = map(soilMoisture, 0, 1023, 0, 100);

  int ldrValue = analogRead(10); // Read the analog value from the LDR
  if(ldrValue == 0){
        digitalWrite(13,HIGH);
  }else{
    digitalWrite(13,LOW);
  }
  if(t>30){
     digitalWrite(0, LOW); 
  }else{
    digitalWrite(0, HIGH); 
  }

   if(50>moisturePercentage){
     digitalWrite(16, LOW); 
  }else{
    digitalWrite(16, HIGH); 
  }
  
  
  if (Firebase.ready() && signupOK ) {
    
    if (Firebase.RTDB.setFloat(&fbdo, "GreenHouse/Values/Humidity",h)){
//      Serial.println("PASSED");
        Serial.print("Humidity: ");
        Serial.print(h);
        Serial.println("%");
      
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "GreenHouse/Values/Temperature", t)){
//      Serial.println("PASSED");
         Serial.print("Temperature: ");
         Serial.print(t);
         Serial.println("°C");
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "GreenHouse/Values/SoilMoisture", moisturePercentage)){
//      Serial.println("PASSED");
        
    }else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    if (Firebase.RTDB.setFloat(&fbdo, "GreenHouse/Values/LDRValue", ldrValue)){
//      Serial.println("PASSED");
         Serial.print("LDR Value: ");
         Serial.print(ldrValue);
         
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
     }
  Serial.println("");
  delay(2000);

}

 
