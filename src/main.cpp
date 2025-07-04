#include <Arduino.h>
#include <Ps3Controller.h>
#include <ESP32Servo.h>

#include "esp_bt_device.h"
#include "steering.h"

Servo steeringServo;
Servo servo2;

#define LED_PIN_ON_BOARD 2
#define HEAD_LIGHTS 23
#define TAIL_LIGHTS 22
#define NEON_MID_LIGHTS 21
#define SERVO_PIN 14
#define SERVO2_PIN 27
#define ENA 4
#define IN1 5
#define IN2 15

#define DRIVE_MOTOR_CHANNEL 15

bool headlightState = false;
bool tailLightState = false;
bool neonMidLightsState = false;
bool ledOnboardState = false;
bool rumbleOn = false;
unsigned long lastBlinkTime = 0;
unsigned long onBoardLedLastBlinkTime = 0;
unsigned long ledOnBoardBlinkCount = 0;
unsigned long rumbleStart = 0;

void onStickLeft() {
  steeringServo.write(0);
}

void onStickRight() {
  steeringServo.write(180);
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

  rumbleStart = millis();
  Ps3.setRumble(50, -1);
  rumbleOn = true;
}

void onStickUp() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  int speed = map(Ps3.data.analog.stick.ry, 0, -127, 0, 255);
  speed = constrain(speed, 0, 255);
  ledcWrite(DRIVE_MOTOR_CHANNEL, speed);
}

void onStickDown() {
    digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  int speed = map(Ps3.data.analog.stick.ry, 0, 127, 0, 255);
  speed = constrain(speed, 0, 255);
  ledcWrite(DRIVE_MOTOR_CHANNEL, speed);
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
  // check headlightState
  unsigned long now = millis();
  if (now - lastXButtonPress > 400) {
    Serial.println("X button pressed");
    if (headlightState) {
      digitalWrite(HEAD_LIGHTS, LOW);
      headlightState = false;
    } else {
      digitalWrite(HEAD_LIGHTS, HIGH);
      headlightState = true;
    }
    lastXButtonPress = now;
  }
}

unsigned long lastSquareButtonPress = 0;

void onSquareButtonPressed() {
  unsigned long now = millis();
  if (now - lastSquareButtonPress > 400) { // debounce time
    // check tailLightState
    if (tailLightState) {
      digitalWrite(TAIL_LIGHTS, LOW);
      tailLightState = false;
    } else {
      digitalWrite(TAIL_LIGHTS, HIGH);
      tailLightState = true;
    }
    Serial.println("Square button pressed");
    lastSquareButtonPress = now; // update last press time
  }
}

unsigned long lastTriangleButtonPress = 0;

void onTriangleButtonPressed() {
  unsigned long now = millis();
  if (now - lastTriangleButtonPress > 400) { // debounce time
    // check neonMidLightsState
    if (neonMidLightsState) {
      digitalWrite(NEON_MID_LIGHTS, LOW);
      neonMidLightsState = false;
    } else {
      digitalWrite(NEON_MID_LIGHTS, HIGH);
      neonMidLightsState = true;
    }
    Serial.println("Triangle button pressed");
    lastTriangleButtonPress = now; // update last press time
  }
}

void notify() {
  if (Ps3.data.button.cross) onXButtonPressed();
  if (Ps3.data.button.circle) Serial.println("O button pressed");
  if (Ps3.data.button.triangle) onTriangleButtonPressed();
  if (Ps3.data.button.square) onSquareButtonPressed();
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
  pinMode(HEAD_LIGHTS, OUTPUT);
  pinMode(LED_PIN_ON_BOARD, OUTPUT);
  pinMode(TAIL_LIGHTS, OUTPUT);
  pinMode(NEON_MID_LIGHTS, OUTPUT);

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
  if (rumbleOn && millis() - rumbleStart > 500) {
    Ps3.setRumble(0, 0);  // Stop it manually
    rumbleOn = false;
  }
}


