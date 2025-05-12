// Pin Definitions
const int motionSensorPin = 2;   // Pin for the motion sensor
const int buzzerPin = 6;          // Pin for the buzzer
const int redPin = 5;             // Pin for the red part of the RGB LED
const int greenPin = 3;           // Pin for the green part of the RGB LED
const int bluePin = 4;            // Pin for the blue part of the RGB LED

// Timing Parameters
const unsigned long buzzerDuration = 3000;  // Buzzer duration in milliseconds (3 seconds)
const unsigned long flashDuration = 10000;  // Flashing duration for the LED (10 seconds)
const unsigned long flashOnTime = 150;      // Time the LED stays ON (150ms)
const unsigned long flashOffTime = 150;     // Time the LED stays OFF (150ms)

unsigned long buzzerStart = 0;  // Start time for buzzer
unsigned long ledStart = 0;     // Start time for LED
unsigned long ledFlashStart = 0; // Start time for LED flashing

bool buzzerOn = false;  // Boolean to track if buzzer is ON
bool ledOn = false;     // Boolean to track if LED is flashing
bool ledState = false;  // Boolean to track LED state (ON/OFF)

int motionCount = 0;     // Counter to track how many times motion has been detected
int currentR = 0;        // Current red intensity for RGB LED
int currentG = 0;        // Current green intensity for RGB LED
int currentB = 0;        // Current blue intensity for RGB LED

void setup() {
  // Initialize pins
  pinMode(motionSensorPin, INPUT);  // Motion sensor pin as input
  pinMode(buzzerPin, OUTPUT);       // Buzzer pin as output
  pinMode(redPin, OUTPUT);          // Red LED pin as output
  pinMode(greenPin, OUTPUT);        // Green LED pin as output
  pinMode(bluePin, OUTPUT);         // Blue LED pin as output

  // Initialize all outputs to LOW (turn off buzzer and LED initially)
  digitalWrite(buzzerPin, LOW);
  setRgbColor(0, 0, 0);  // Turn off the RGB LED

  Serial.begin(9600);  // Start serial communication for debugging
}

void loop() {
  unsigned long now = millis();  // Get current time in milliseconds
  bool motion = digitalRead(motionSensorPin) == HIGH;  // Check if motion is detected

  // When motion is detected and buzzer is not already on
  if (motion && !buzzerOn) {
    buzzerOn = true;
    buzzerStart = now;  // Record the start time for the buzzer
    digitalWrite(buzzerPin, HIGH);  // Turn on the buzzer
    motionCount++;  // Increment the motion detection count

    Serial.print("Motion Detected! Count = ");
    Serial.println(motionCount);

    // Set LED color based on motion count
    if (motionCount <= 3) {
      currentR = 0; currentG = 255; currentB = 0;  // Green for first 3 motions
    } else {
      currentR = 255; currentG = 0; currentB = 0;  // Red for 4th and beyond
    }

    ledOn = true;  // Start the LED flashing
    ledState = true;  // Start with the LED ON
    ledFlashStart = now;  // Record the flash start time
    ledStart = now;  // Start the LED timer
    setRgbColor(currentR, currentG, currentB);  // Set LED to current color
  }

  // Handle buzzer OFF after 3 seconds
  if (buzzerOn && (now - buzzerStart >= buzzerDuration)) {
    digitalWrite(buzzerPin, LOW);  // Turn off the buzzer
    buzzerOn = false;  // Mark buzzer as OFF
    Serial.println("Buzzer Off");
  }

  // Handle LED flashing
  if (ledOn) {
    if (now - ledFlashStart >= flashDuration) {
      ledOn = false;  // Stop the LED after 10 seconds
      setRgbColor(0, 0, 0);  // Turn off the LED
      Serial.println("LED Flashing Done");
    } else {
      // Flashing the LED ON and OFF
      if (ledState && now - ledStart >= flashOnTime) {
        ledState = false;  // Turn LED OFF
        setRgbColor(0, 0, 0);  // Set RGB LED to OFF
        ledStart = now;  // Reset the start time
      } else if (!ledState && now - ledStart >= flashOffTime) {
        ledState = true;  // Turn LED ON
        setRgbColor(currentR, currentG, currentB);  // Set RGB LED to current color
        ledStart = now;  // Reset the start time
      }
    }
  }
}

// Helper function to set the RGB LED color
void setRgbColor(int r, int g, int b) {
  analogWrite(redPin, r);   // Set the red pin brightness
  analogWrite(greenPin, g); // Set the green pin brightness
  analogWrite(bluePin, b);  // Set the blue pin brightness
}
