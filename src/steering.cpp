#include <ESP32Servo.h>

extern Servo steeringServo;

#define SERVO_PIN 14

void setupServo() {
  int steeringServoChannel = steeringServo.attach(SERVO_PIN);
  Serial.print("ℹ️ Steering servo attached to channel: ");
  Serial.println(steeringServoChannel);

  steeringServo.write(90);
  steeringServo.setPeriodHertz(50);
  steeringServo.attach(SERVO_PIN, 200, 3000);
}
