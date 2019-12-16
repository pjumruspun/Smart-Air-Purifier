#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>
#include <string.h>
#include <String>

ESP8266WiFiMulti WiFiMulti;

#define URL "http://08558270.ngrok.io/status_request"
#define analogPin 0
#define digitalPin 5
#define ledPin 4
#define ssrPin 15

float sensorValue = 0;
float calcVoltage = 0;
float dustDensity = 0;
//float accumulatedValue = 0;

int receivedInt = -1;

int autoMode = 0;
const float dustThreshold = 0.30;
const int MAX_IGN_VARIABLE = 10;
int ignoringVariable = 0;
float lastDustLevel = 0;

int inProgress = 0;
const char startChar = '<';
const char endChar = '>';

const byte numberOfChar = 64;
char receivedStr[numberOfChar];
byte idx = 0;


int iter = 0;
SoftwareSerial mySerial(D6, D5); // RX | TX

void setup() {
  // put your setup code here, to run once:
  //pinMode(ledPin, OUTPUT);
  pinMode(digitalPin, INPUT);
  pinMode(ssrPin, OUTPUT);
  //pinMode(analogPin, INPUT);
  Serial.begin(115200); 
  pinMode(D6, INPUT);
  pinMode(D5, OUTPUT);
  mySerial.begin(115200);
  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  //digitalWrite(ledPin, HIGH);
  //digitalWrite(ssrPin, HIGH);
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("SprkZ", "0000000000");
  Serial.print("Connecting");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nConnected!!");
}

void loop() {
  /*
  digitalWrite(ledPin, LOW);
  delayMicroseconds(280);
  sensorValue = analogRead(analogPin);
  delayMicroseconds(40);
  digitalWrite(ledPin, HIGH);
  delayMicroseconds(9680);
  accumulatedValue += sensorValue;
  if(iter%100 == 0){
    accumulatedValue /= 100;
    calcVoltage = accumulatedValue*(5.0/1024);
    dustDensity = 0.17*calcVoltage-0.1;
    processAutoMode(dustDensity);
    
    //Serial.printf("Analog raw = %f, CalcVoltage = %f, DustDensity = %f\n", accumulatedValue, calcVoltage, dustDensity);
    accumulatedValue = 0;
  }
  */

  
  
  while (mySerial.available() > 0){
    char receivedChar = mySerial.read();
    //Serial.println("I'm in!");
    //Serial.println(receivedChar);
    if(inProgress == 1){
      if(receivedChar != endChar){
        receivedStr[idx] = receivedChar;
        idx++;
        if(idx >= numberOfChar)
          idx = numberOfChar - 1;
      }
      else{
        receivedStr[idx] = 0;
        inProgress = 0;
        idx = 0;
        //Serial.println(receivedStr);
        sscanf(receivedStr, "%d", &receivedInt);
        calcVoltage = receivedInt*(5.0/1024);
        dustDensity = 0.17*calcVoltage-0.1;
        Serial.printf("Dust density = %f\n", dustDensity);
        
      }
    }
    else if(receivedChar == startChar){
      inProgress = 1;
    }
  }

  if(iter%100 == 0) processAutoMode(dustDensity); 
  
  if ((WiFiMulti.run() == WL_CONNECTED) && iter%100 == 0) {
    //Serial.println("CONNECTED!!!");
    
    char token[10];
    HTTPClient http;
  
    //Serial.print("[HTTP] begin...\n");

    String realURL = URL;
    realURL.concat("?dust_level=");
    realURL.concat(String(dustDensity));
    Serial.println(realURL);
    http.begin(realURL); //HTTP
    //Serial.println(URL);
    
    //Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    //Serial.printf("code: %d\n", httpCode);
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        String useless = http.getString();
        char payload[50];
        useless.toCharArray(payload, 50);
        //Serial.println(payload);
        processStatus(payload, dustDensity);
      
      } else {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    http.end();
    }
  }
  //delay(500);
  iter++;
} 

void processStatus(char* statusRequest, float dustLevel){
  Serial.printf("Status processing: %s\n", statusRequest);
  if(strcmp(statusRequest, "on") == 0){
    if(autoMode){
      Serial.println("\tForcing manual mode...");
      autoMode = 0;
    }
     Serial.println("turning the device on!");
     digitalWrite(ssrPin, HIGH);
  }
  else if(strcmp(statusRequest, "off") == 0){
    if(autoMode){
      Serial.println("\tForcing manual mode...");
      autoMode = 0;
    }
    Serial.println("turning the device off!");
    digitalWrite(ssrPin, LOW);
  }
  else if(strcmp(statusRequest, "auto") == 0){
    if(!autoMode){
      Serial.println("turning the auto mode on!");
      autoMode = 1;
      digitalWrite(ssrPin, LOW);
    }
  }
  else if(strcmp(statusRequest, "manual") == 0){
    Serial.println("turning the auto mode off!");
    autoMode = 0;
  }
  else if(strcmp(statusRequest, "idle") == 0){
    Serial.println("Idling...");
  }
  else{
    Serial.println("Status error");
  }
}

void processAutoMode(float dustLevel){
  
  if(autoMode){
    if(dustLevel > dustThreshold){
      Serial.println("turning the device on by auto mode!");
      ignoringVariable = MAX_IGN_VARIABLE;
    }
    if(ignoringVariable > 0){
      digitalWrite(ssrPin, HIGH);
      Serial.println("\t still turned on...");
      ignoringVariable--;
    }
    else{
      digitalWrite(ssrPin, LOW);
      Serial.println("System turned off by auto mode!");
      ignoringVariable = 0;
    }
  }
}
