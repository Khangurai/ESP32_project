//#include <analogWrite.h>

// Define the LED pin
const int ledPin = 2;

// Define the fade amount
const int fadeAmount = 5;

void setup() {
  // Set the LED pin as an output
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Fade the LED up
  for (int brightness = 0; brightness <= 255; brightness += fadeAmount) {
    analogWrite(ledPin, brightness);
    delay(10);
  }

  // Fade the LED down
  for (int brightness = 255; brightness >= 0; brightness -= fadeAmount) {
    analogWrite(ledPin, brightness);
    delay(10);
  }
}