#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello, World!");
  Serial.println("Result of myFunction(2, 3): ");
  delay(1000); // Delay for 1 second
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}
