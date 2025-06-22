#include <Arduino.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>

#include "esp_bt_device.h"
#include "steering.h"

Servo steeringServo;
Servo servo2;

#define LED_PIN 4
#define SERVO2_PIN 27

bool ledState = false;
unsigned long lastBlinkTime = 0;

void onLeftButtonPressed() {
  Serial.println("Left button pressed");
  steeringServo.write(180);
}

void onRightButtonPressed() {
  Serial.println("Right button pressed");
  steeringServo.write(0);
}

unsigned long lastUpButtonPress = 0;

void onUpButtonPressed() {
  unsigned long now = millis();
  if (now - lastUpButtonPress > 400) {
    Serial.println("Up button pressed");
    servo2.write(180);
    delay(95);
    servo2.write(90);
    lastUpButtonPress = now;
  }
}

unsigned long lastDownButtonPress = 0;

void onDownButtonPressed() {
  unsigned long now = millis();
  if (now - lastDownButtonPress > 400) {
    Serial.println("Down button pressed");
    servo2.write(0);
    delay(95);
    servo2.write(90);
    lastDownButtonPress = now;
  }
}

void testServo() {
  Serial.println("🚀 Starting servo test...");

  for (int i = 0; i <= 360; i += 10) {
    steeringServo.write(i);
    Serial.print("Moving to ");
    Serial.print(i);
    Serial.println(" degrees...");
    delay(300);
  }

  Serial.println("Stopping.");
}

void setupServo2() {
  Serial.println("Setting up servo 2...");
  servo2.attach(SERVO2_PIN);
  servo2.setPeriodHertz(50);
  servo2.attach(SERVO2_PIN, 1000, 2000);
  servo2.write(90);
}


void notify() {
  if (Ps3.data.button.cross) Serial.println("X button pressed");
  if (Ps3.data.button.circle) Serial.println("O button pressed");
  if (Ps3.data.button.triangle) Serial.println("Triangle button pressed");
  if (Ps3.data.button.square) Serial.println("Square button pressed");
  if (Ps3.data.button.select) Serial.println("Select button pressed");
  if (Ps3.data.button.start) Serial.println("Start button pressed");
  if (Ps3.data.button.up) onUpButtonPressed();
  if (Ps3.data.button.down) onDownButtonPressed();
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
    steeringServo.write(90);
  }

  if (!Ps3.data.button.up && !Ps3.data.button.down) {
    servo2.write(90);
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
  setupServo2();

  steeringServo.write(90);
  //testServo();
}

void loop() {
  // Nothing for now
}


