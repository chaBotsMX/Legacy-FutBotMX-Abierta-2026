#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#define NUMPIXELS 18
#define PIN 4
#define VREFS 4
#define SENSOR_COUNT 8
#define DETECTED_STATE HIGH

int vref[VREFS] = {2,3,22,23};
int frontPins[SENSOR_COUNT] = {14,15,16,17,18,19,20,21};
int backPins[SENSOR_COUNT] = {5,6,7,8,9,12,11,10};
int frontCos[NUMPIXELS/2] = {};
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int getSensorDepth(int index) {
  return min(index + 1, SENSOR_COUNT - index);
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(2000000);
  for(int i = 0 ; i < VREFS; i++){
    pinMode(vref[i], OUTPUT);
  }
  for(int i = 0 ; i < 8; i++){
    pinMode(backPins[i],INPUT);
  }

  for(int i = 0 ; i < 8; i++){
    pinMode(frontPins[i],INPUT);
  }
  pixels.begin();
  for(int i=0; i<NUMPIXELS; i++) { 

    pixels.setPixelColor(i, pixels.Color(100, 100, 100));
    pixels.show();   
    delay(100);
  }
  for(int i = 0; i <VREFS ; i++){
    analogWrite(vref[i],180);
  }
}

void loop() {
  int sendData = 500;
  int max = 0;
  for(int i = 0; i<SENSOR_COUNT ;i++){
    int input = digitalRead(frontPins[i]);
    if(input == DETECTED_STATE){
      int depth = getSensorDepth(i);
      if(depth > max){
        max = depth;
        sendData = depth;
      }
      /*Serial.print(" F");
      Serial.print(i);
      Serial.print(":");
      Serial.print(getSensorDepth(i));*/
    }
  }
  if(sendData == 500){
    for(int i = 0; i<SENSOR_COUNT ;i++){
      int input = digitalRead(backPins[i]);
      if(input == DETECTED_STATE){
        int depth = getSensorDepth(i);
        if(depth > max){
          max = depth;
          sendData = depth + 10;
        }
        /*Serial.print(" B");
        Serial.print(i);
        Serial.print(":");
        Serial.print(getSensorDepth(i));*/
      }
    }
  }
  Serial1.write(sendData);

}
