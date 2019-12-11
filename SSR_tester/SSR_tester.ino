#include <SoftwareSerial.h>
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;
SoftwareSerial NodeSerial(D2, D3); // RX | TX

#define URL "http://bc788930.ngrok.io/status_request"

void setup() {
  // put your setup code here, to run once:
  pinMode(D2, INPUT);
  pinMode(D3, OUTPUT);
  pinMode(15, OUTPUT);
  Serial.begin(115200);
  NodeSerial.begin(115200);
  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("No free wifi for you uwu", "87654321");
}

void loop() {
  if ((WiFiMulti.run() == WL_CONNECTED)) {
    USE_SERIAL.println("CONNECTED!!!");
    
    char token[10];
    HTTPClient http;
  
    USE_SERIAL.print("[HTTP] begin...\n");
    
    http.begin(URL); //HTTP
    Serial.println(URL);
    
    USE_SERIAL.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    USE_SERIAL.printf("code: %d\n", httpCode);
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      USE_SERIAL.printf("code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        USE_SERIAL.println(payload);
        
      
    } else {
      USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  
  
  
    }
  }
  delay(5000);
  /*
  while (NodeSerial.available() > 0)
  {
    Serial.println("yay");
    
    int i_data = NodeSerial.parseInt();
    float f_data = NodeSerial.parseFloat();
    if (NodeSerial.read() == '\n')
    {
      Serial.print("NodeMCU or ESP8266");
      Serial.print(" : ");
      Serial.print(i_data); Serial.print(" : ");
      Serial.println(f_data);
    }
  }*/
}
