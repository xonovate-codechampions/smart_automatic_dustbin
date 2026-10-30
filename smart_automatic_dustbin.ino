
#include <Servo.h>

// ==========================================
// SMART AUTOMATIC DUSTBIN
// Xonovate Code Champions (XCC)
// ==========================================

// Servo motor
Servo lidServo;
const int servoPin = 9;

// Ultrasonic sensor (HC-SR04)
const int trigPin = 7;
const int echoPin = 6;

// Lid positions
const int LID_OPEN = 0;
const int LID_CLOSED = 90;

// Detection settings
const int detectionDistance = 12; // centimetres
const unsigned long clearDelay = 3000; // 3 seconds
const unsigned long sensorTimeout = 30000; // microseconds

// Lid state
bool lidIsOpen = false;
unsigned long lastObjectDetection = 0;


// ==========================================
// FUNCTION: GET DISTANCE
// Measures distance between the sensor
// and a detected object.
// ==========================================

float getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, sensorTimeout);

  if (duration == 0) {
    return 999; // No echo received
  }

  return duration * 0.0343 / 2;
}


// ==========================================
// FUNCTION: OPEN LID
// Moves the servo smoothly from 90 to 0.
// ==========================================

void openLid() {

  Serial.println("Opening dustbin lid...");

  for (int angle = LID_CLOSED; angle >= LID_OPEN; angle--) {
    lidServo.write(angle);
    delay(15);
  }

  lidIsOpen = true;

  Serial.println("Lid OPEN");
}


// ==========================================
// FUNCTION: CLOSE LID
// Moves the servo smoothly from 0 to 90.
// ==========================================

void closeLid() {

  Serial.println("Closing dustbin lid...");

  for (int angle = LID_OPEN; angle <= LID_CLOSED; angle++) {
    lidServo.write(angle);
    delay(15);
  }

  lidIsOpen = false;

  Serial.println("Lid CLOSED");
}


// ==========================================
// SETUP
// ==========================================

void setup() {

  Serial.begin(9600);

  // Configure sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Attach servo motor
  lidServo.attach(servoPin);

  // Initialise lid in closed position
  lidServo.write(LID_CLOSED);

  Serial.println("================================");
  Serial.println("SMART AUTOMATIC DUSTBIN");
  Serial.println("Xonovate Code Champions");
  Serial.println("================================");
  Serial.println("System ready. Waiting for object...");
}


// ==========================================
// MAIN LOOP
// ==========================================

void loop() {

  float distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Check whether an object is within range
  if (distance <= detectionDistance) {

    lastObjectDetection = millis();

    // Open lid if currently closed
    if (!lidIsOpen) {
      openLid();
    }

    Serial.println("Object detected");

  }

  // Check whether the lid should close
  if (lidIsOpen) {

    unsigned long elapsedTime =
      millis() - lastObjectDetection;

    // Close only after the object has been
    // absent for the specified duration
    if (elapsedTime >= clearDelay) {

      closeLid();

      Serial.println("Object cleared.");
      Serial.println("Waiting for next object...");
    }
  }

  delay(100);
}
