#define STEP_PIN 16
#define DIR_PIN 17
#define EN_PIN 18
#define HOME_PIN 4

#define STEPS_PER_SAMPLE 25

// IR sensor:
// LOW  = home detected
// HIGH = home not detected

// ---------------- STEP FUNCTION ----------------
void stepOnce(int delayUs) {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(delayUs);

  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(delayUs);
}

// ---------------- MOVE STEPS ----------------
void rotateSteps(int steps, bool direction) {

  digitalWrite(DIR_PIN, direction ? HIGH : LOW);

  for (int i = 0; i < steps; i++) {
    stepOnce(800);
    delay(5);
  }
}

// ---------------- HOMING ----------------
void homeMotor() {

  Serial.println("HOMING START");

  digitalWrite(EN_PIN, LOW);

  // Check whether sensor is already triggered

  if (digitalRead(HOME_PIN) == LOW) {

    Serial.println("Sensor already active - moving away");

    // Move away from home
    digitalWrite(DIR_PIN, HIGH);

    for (int i = 0; i < 30; i++) {
      stepOnce(2000);
    }

    delay(100);
  }


  // FAST SEARCH FOR HOME
  Serial.println("Fast homing...");

  digitalWrite(DIR_PIN, LOW);

  while (digitalRead(HOME_PIN) == HIGH) {
    stepOnce(2000);
  }

  Serial.println("Home detected");

  delay(50);

  Serial.println("Backoff...");

  digitalWrite(DIR_PIN, HIGH);

  for (int i = 0; i < 30; i++) {
    stepOnce(2000);
  }

  delay(100);

  // SLOW FINAL APPROACH
  Serial.println("Slow approach...");

  digitalWrite(DIR_PIN, LOW);

  while (digitalRead(HOME_PIN) == HIGH) {
    stepOnce(1200);
  }

  delay(50);

  Serial.println("HOME SET");
}

// ---------------- PL SCAN ----------------
void runPLScan() {

  Serial.println("SCAN START");

  for (int i = 0; i < 8; i++) {

    Serial.print("Sample ");
    Serial.println(i + 1);

    delay(2000);

    if (i < 7) {
      rotateSteps(STEPS_PER_SAMPLE, false);
    }
  }

  Serial.println("SCAN COMPLETE");
}

// ---------------- SETUP ----------------
void setup() {

  Serial.begin(115200);
  delay(2000);

  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  // IR sensor output
  pinMode(HOME_PIN, INPUT_PULLUP);

  digitalWrite(EN_PIN, LOW);

  Serial.println("SYSTEM START");

  homeMotor();

  runPLScan();

  digitalWrite(EN_PIN, HIGH);

  Serial.println("SYSTEM DONE");
}

// ---------------- LOOP ----------------
void loop() {
  // Nothing - system runs once
}