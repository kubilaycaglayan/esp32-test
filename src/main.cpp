#include <Arduino.h>
#include "esp_bt_device.h"
#include <Ps3Controller.h>

#define LED_PIN 4  // Pin for the LED
bool ledState = false;
unsigned long lastBlinkTime = 0;

void notify() {
  Serial.println("Data received!");

  if (Ps3.data.button.cross) Serial.println("X button pressed");
  if (Ps3.data.button.circle) Serial.println("O button pressed");
  if (Ps3.data.button.triangle) Serial.println("Triangle button pressed");
  if (Ps3.data.button.square) Serial.println("Square button pressed");

    if (Ps3.data.button.cross) {
    unsigned long now = millis();

    // Blink every 500ms while X is held
    if (now - lastBlinkTime > 500) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      lastBlinkTime = now;
    }
  } else {
    digitalWrite(LED_PIN, LOW); // Turn off when X is released
    ledState = false;
  }
}

void onConnect() {
  Serial.println("🎮 PS3 controller connected!");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("3C:8A:1F:AF:7F:D2");  // Replace with your MAC
}

void loop() {
  // Nothing here; all handled by events
}



