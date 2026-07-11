#include <esp_now.h>
#include <WiFi.h>

int buttonPin = 23;
int callPin = 18;
bool lockedout = true;

uint8_t hubAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};

void setup() {
  //Setup
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(callPin, OUTPUT);
  //End Setup

  //Button Logic
    //request lockout value
    //lockedout = response
    if (!lockedout) {
      digitalWrite(callPin, HIGH);
      delay(20);
      digitalWrite(callPin, LOW);
    }

  //Sleep Call
  esp_sleep_enable_ext0_wakeup(23, HIGH);
  esp_deep_sleep_start();
}

void loop() {
}