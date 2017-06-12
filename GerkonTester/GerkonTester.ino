#define OPEN_PIN 13
#define OPEN_60_PIN 12
#define OPEN_30_PIN 11
#define CLOSE_PIN 10

#define OPEN_SIGNAL_PIN 4
#define STOP_SIGNAL_PIN 3
#define CLOSE_SIGNAL_PIN 2

int gableState = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(OPEN_PIN, INPUT);
  pinMode(OPEN_60_PIN, INPUT);
  pinMode(OPEN_30_PIN, INPUT);
  pinMode(CLOSE_PIN, INPUT);
   pinMode(OPEN_SIGNAL_PIN, OUTPUT);
  digitalWrite(OPEN_SIGNAL_PIN, HIGH);
  pinMode(STOP_SIGNAL_PIN, OUTPUT);
  digitalWrite(STOP_SIGNAL_PIN, HIGH);
  pinMode(CLOSE_SIGNAL_PIN, OUTPUT);
  digitalWrite(CLOSE_SIGNAL_PIN, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  // send pin state
  if(digitalRead(OPEN_PIN) > 0) {
    if(gableState != 100){
      gableState = 100;
      Serial.println("Open");
    }
  } else if(digitalRead(OPEN_60_PIN) > 0) {
    if(gableState != 60){
      gableState = 60;
      Serial.println("Open 60");
    }
  } else if(digitalRead(OPEN_30_PIN) > 0) {
    if(gableState != 30){
      gableState = 30;
      Serial.println("Open 30");
    }
  } else if(digitalRead(CLOSE_PIN) > 0) {
    if(gableState != 0){
      gableState = 0;
      Serial.println("Close");
    }
  } else {
    Serial.println("---------");
    gableState = -1;
  }
  delay(250);        // delay in between reads for stability
}
