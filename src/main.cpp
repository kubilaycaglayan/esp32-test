#include <Arduino.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>

#include "esp_bt_device.h"
#include "steering.h"

Servo steeringServo;
Servo servo2;

#define LED_PIN_ON_BOARD 2
#define LED_PIN 23
#define SERVO2_PIN 27
#define ENA 4
#define IN1 5
#define IN2 15

#define DRIVE_MOTOR_CHANNEL 15

bool ledState = false;
bool ledOnboardState = false;
unsigned long lastBlinkTime = 0;
unsigned long onBoardLedLastBlinkTime = 0;
unsigned long ledOnBoardBlinkCount = 0;

void onStickLeft() {
  steeringServo.write(180);
}

void onStickRight() {
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

void onStickUp() {
  Serial.println("Analog stick pulled up");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  int speed = map(Ps3.data.analog.stick.ry, 0, -127, 0, 255);
  speed = constrain(speed, 0, 255); // Ensure speed is within valid range
  ledcWrite(DRIVE_MOTOR_CHANNEL, speed); // Set speed based on joystick position
  Serial.print("Setting motor speed to: ");
  Serial.println(speed);

}

void onStickDown() {
  Serial.println("Analog stick pulled down");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // ledcWrite(DRIVE_MOTOR_CHANNEL, 255); // Full speed backward
  int speed = map(Ps3.data.analog.stick.ry, 0, 127, 0, 255);
  speed = constrain(speed, 0, 255); // Ensure speed is within valid range
  ledcWrite(DRIVE_MOTOR_CHANNEL, speed); // Set speed based on joystick position
  Serial.print("Setting motor speed to: ");
  Serial.println(speed);

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
  int servo2Channel = servo2.attach(SERVO2_PIN);
  Serial.print("ℹ️ Servo 2 attached to channel: ");
  Serial.println(servo2Channel);
  servo2.setPeriodHertz(50);
  int servo2Channel_2 = servo2.attach(SERVO2_PIN, 1000, 2000);
  Serial.print("ℹ️ Servo 2 attached to channel (with range): ");
  Serial.println(servo2Channel_2);
  servo2.write(90);
}

unsigned long lastXButtonPress = 0;

void onXButtonPressed() {
  // check ledState
  unsigned long now = millis();
  if (now - lastXButtonPress > 400) {
    Serial.println("X button pressed");
    if (ledState) {
      digitalWrite(LED_PIN, LOW);
      ledState = false;
    } else {
      digitalWrite(LED_PIN, HIGH);
      ledState = true;
    }
    lastXButtonPress = now;
  }
}

void notify() {
  if (Ps3.data.button.cross) onXButtonPressed();
  if (Ps3.data.button.circle) Serial.println("O button pressed");
  if (Ps3.data.button.triangle) Serial.println("Triangle button pressed");
  if (Ps3.data.button.square) Serial.println("Square button pressed");
  if (Ps3.data.button.select) Serial.println("Select button pressed");
  if (Ps3.data.button.start) Serial.println("Start button pressed");
  if (Ps3.data.button.up) onUpButtonPressed();
  if (Ps3.data.button.down) onDownButtonPressed();
  if (Ps3.data.button.left) Serial.println("Left button pressed");
  if (Ps3.data.button.right) Serial.println("Right button pressed");
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

  if ( Ps3.data.analog.stick.rx < -5 ) {
    onStickLeft();
  }

  if ( Ps3.data.analog.stick.rx > 5 ) {
    onStickRight();
  }

  if ( Ps3.data.analog.stick.ry > 5 ) {
    Serial.println(Ps3.data.analog.stick.ry);
    Serial.println("Currentry pulling analog stick to down");
    onStickDown();
  }

  if ( Ps3.data.analog.stick.ry < -5 ) {
    Serial.println(Ps3.data.analog.stick.ry);
    Serial.println("Currently pulling analog stick to up");
    onStickUp();
  }

  if (abs(Ps3.data.analog.stick.ry) < 5) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    ledcWrite(DRIVE_MOTOR_CHANNEL, 0); // Stop the motor
  }


  if (abs(Ps3.data.analog.stick.rx) < 5) {
    steeringServo.write(90);
  }

  if (!Ps3.data.button.up && !Ps3.data.button.down) {
    servo2.write(90);
  }
}


void blinkOnBoardLedThenReturnLastState(int blinkTimes, int delayTime = 500) {
  unsigned stateToRestore = ledOnboardState;

  for (int i = 0; i < blinkTimes; i++) {
    if (ledOnboardState) {
      digitalWrite(LED_PIN_ON_BOARD, LOW);
      ledOnboardState = false;
    } else {
      digitalWrite(LED_PIN_ON_BOARD, HIGH);
      ledOnboardState = true;
    }
    delay(delayTime);
  }

  if (stateToRestore) {
    digitalWrite(LED_PIN_ON_BOARD, HIGH);
    ledOnboardState = true;
  } else {
    digitalWrite(LED_PIN_ON_BOARD, LOW);
    ledOnboardState = false;
  }
}

void onConnect() {
  Serial.println("🎮 PS3 controller connected!");
  blinkOnBoardLedThenReturnLastState(3, 500);
}

void setupDriveMotor() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Set direction
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Set speed
  ledcAttachPin(ENA, DRIVE_MOTOR_CHANNEL);
  Serial.print("ℹ️ Drive motor attached to channel: ");
  Serial.println(DRIVE_MOTOR_CHANNEL);

  ledcSetup(DRIVE_MOTOR_CHANNEL, 1000, 8);
  ledcWrite(DRIVE_MOTOR_CHANNEL, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_PIN_ON_BOARD, OUTPUT);

  digitalWrite(LED_PIN_ON_BOARD, HIGH);
  ledOnboardState = true;

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("3C:8A:1F:AF:7F:D2");

  setupServo();
  setupServo2();
  setupDriveMotor();
}

void loop() {

}


