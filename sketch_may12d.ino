#include <NewPing.h>
const int trigPin = 8;
const int echoPin = 9;
const int buzzerPin = 6;
const int redPin = 5;
const int greenPin = 3;
const int bluePin = 4;

// Constants
const int detectionDistance = 5;             // cm
const unsigned long cooldownTime = 1500;     // ms
const unsigned long ledFlashDuration = 300;  // ms
const unsigned long buzzerDuration = 300;    // ms

// Variables
int detectionCount = 0;
unsigned long lastDetectionTime = 0;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(buzzerPin, LOW);  // Ensure buzzer is off at start
  setRgbColor(0, 0, 0);          // Turn off LED

  Serial.begin(9600);
}

void loop() {
  long distance = getDistance();

  if (distance > 0 && distance <= detectionDistance && millis() - lastDetectionTime > cooldownTime) {
    detectionCount++;
    lastDetectionTime = millis();

    Serial.print("Detection #");
    Serial.println(detectionCount);

    // Turn buzzer ON
    digitalWrite(buzzerPin, HIGH);
    delay(buzzerDuration);
    digitalWrite(buzzerPin, LOW);  // Turn buzzer OFF

    // Flash LED
    if (detectionCount <= 3) {
      flashLed(0, 255, 0);  // Green
    } else if (detectionCount <= 10) {
      flashLed(255, 0, 0);  // Red
    }

    // Reset after 10 detections
    if (detectionCount >= 10) {
      detectionCount = 0;
      Serial.println("Resetting detection count.");
    }
  } else {
    // Keep everything off when nothing is detected
    digitalWrite(buzzerPin, LOW);
    setRgbColor(0, 0, 0);
  }
}

void flashLed(int r, int g, int b) {
  setRgbColor(r, g, b);
  delay(ledFlashDuration);
  setRgbColor(0, 0, 0);
}

void setRgbColor(int r, int g, int b) {
  analogWrite(redPin, r);
  analogWrite(greenPin, g);
  analogWrite(bluePin, b);
}

long getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);  // Timeout: 30ms
  long distance = duration * 0.034 / 2;

  if (distance == 0 || distance > 400) return -1;  // Invalid reading
  return distance;
}
