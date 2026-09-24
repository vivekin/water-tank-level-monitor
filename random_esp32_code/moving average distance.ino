//https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home

int trigPin = 5;   // Trigger
int echoPin = 18;  // Echo
long duration, cm;
#define WINDOW_SIZE 5

int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;
void setup()

{
  //Serial Port begin
  Serial.begin(115200);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop()

{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  // Convert the time into a distance

  cm = (duration / 2) / 29.1;  // Divide by 29.1 or multiply by 0.0343
  if (cm < 400) {

    SUM = SUM - READINGS[INDEX];        // Remove the oldest entry from the sum
    VALUE = cm;                         // Read the next sensor value
    READINGS[INDEX] = VALUE;            // Add the newest reading to the window
    SUM = SUM + VALUE;                  // Add the newest reading to the sum
    INDEX = (INDEX + 1) % WINDOW_SIZE;  // Increment the index, and wrap to 0 if it exceeds the window size

    AVERAGED = SUM / WINDOW_SIZE;  // Divide the sum of the window by the window size for the result
  }
  Serial.print(AVERAGED);
  Serial.println(" cm");
  delay(500);
}
