#define LED_BUILTIN 2 // define the GPIO 2 as LED_BUILTIN
void setup() {
  pinMode(LED_BUILTIN, OUTPUT); // initialize GPIO pin 2 LED_BUILTIN as an output.
}
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
  delay(1000);                       // wait for a second
}
