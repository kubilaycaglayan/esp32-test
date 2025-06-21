#include <Arduino.h>


// Blink the led on ESP32 board
#define LED_BUILTIN 2 // Define the built-in LED pin for ESP32
#define LED_PIN 4

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the built-in LED pin as an output
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(LED_PIN, LOW);
  delay(1000);

}

