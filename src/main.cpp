#include <Arduino.h>
#include "esp_bt_device.h"
#include <Ps3Controller.h>
#include <ESP32Servo.h>

Servo myServo;

#define LED_PIN 4  // Pin for the LED
#define SERVO_PIN 14  // GPIO pin connected to servo signal

bool ledState = false;
unsigned long lastBlinkTime = 0;

void onLeftButtonPressed() {
  Serial.println("Left button pressed");
  myServo.write(180);
}

void onRightButtonPressed() {
  Serial.println("Right button pressed");
  myServo.write(0);
}


void testServo() {
  Serial.println("🚀 Starting servo test...");


  Serial.println("Spinning...");

  delay(4000);

  myServo.write(0);  // Move to 0 degrees
  Serial.println("Moving to 0 degrees...");

  for (int i = 0; i <= 360; i += 10) {
    myServo.write(i);  // Move servo to each position
    Serial.print("Moving to ");
    Serial.print(i);
    Serial.println(" degrees...");
    delay(300);  // Wait for servo to reach position
  }

  delay(7000);
/*
  myServo.write(180);  // Move to 180 degrees
  Serial.println("Moving to 180 degrees...");
  delay(7000);

  myServo.write(90);  // Move back to 90 degrees
  Serial.println("Moving back to 90 degrees...");
  delay(7000); */


  Serial.println("Stopping.");
}

void setupServo() {
  myServo.attach(SERVO_PIN);
  myServo.write(90);  // Stop servo
  myServo.setPeriodHertz(50);  // Standard 50Hz for servos
  myServo.attach(SERVO_PIN, 1000, 2000);  // min/max pulse width in µs
}


void notify() {
  if (Ps3.data.button.cross) Serial.println("X button pressed");
  if (Ps3.data.button.circle) Serial.println("O button pressed");
  if (Ps3.data.button.triangle) Serial.println("Triangle button pressed");
  if (Ps3.data.button.square) Serial.println("Square button pressed");
  if (Ps3.data.button.select) Serial.println("Select button pressed");
  if (Ps3.data.button.start) Serial.println("Start button pressed");
  if (Ps3.data.button.up) Serial.println("Up button pressed");
  if (Ps3.data.button.down) Serial.println("Down button pressed");
  if (Ps3.data.button.left) onLeftButtonPressed();
  if (Ps3.data.button.right) onRightButtonPressed();
  if (Ps3.data.button.l1) Serial.println("L1 button pressed");
  if (Ps3.data.button.l2) Serial.println("L2 button pressed");
  if (Ps3.data.button.r1) Serial.println("R1 button pressed");
  if (Ps3.data.button.r2) Serial.println("R2 button pressed");
  if (Ps3.data.button.ps) Serial.println("PS button pressed");
  if (Ps3.data.button.l3) Serial.println("L3 button pressed");
  if (Ps3.data.button.r3) Serial.println("R3 button pressed");
  if (Ps3.data.button.up && Ps3.data.button.right) Serial.println("Up and Right pressed together");
  if (Ps3.data.button.left && Ps3.data.button.right) Serial.println("Left and Right pressed together");
  if (Ps3.data.button.l3 && Ps3.data.button.r3) Serial.println("L3 and R3 pressed together");
  if (Ps3.data.button.up && Ps3.data.button.left) Serial.println("Up and Left pressed together");

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

  if (!Ps3.data.button.left && !Ps3.data.button.right) {
    myServo.write(90);
    Serial.println("Returning servo to center (90°)");
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
  setupServo();

  myServo.write(90);
  testServo();
}

void loop() {
  // Nothing here; all handled by events
}


