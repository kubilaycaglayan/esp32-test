#include <Arduino.h>
#include "esp_bt_device.h"
#include <Ps3Controller.h>
#include <ESP32Servo.h>

Servo myServo;

#define LED_PIN 4
#define SERVO_PIN 14

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

  Serial.println("Moving to 0 degrees...");

  for (int i = 0; i <= 360; i += 10) {
    myServo.write(i);
    Serial.print("Moving to ");
    Serial.print(i);
    Serial.println(" degrees...");
    delay(300);
  }
  Serial.println("Stopping.");
}

void setupServo() {
  myServo.attach(SERVO_PIN);
  myServo.write(90);
  myServo.setPeriodHertz(50);
  myServo.attach(SERVO_PIN, 200, 3000);
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

    if (now - lastBlinkTime > 500) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState ? HIGH : LOW);
      lastBlinkTime = now;
    }
  } else {
    digitalWrite(LED_PIN, LOW);
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
  Ps3.begin("3C:8A:1F:AF:7F:D2");
  setupServo();

  myServo.write(90);
  testServo();
}

void loop() {
  // Nothing for now
}


