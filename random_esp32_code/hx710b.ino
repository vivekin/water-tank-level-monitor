// https://github.com/bogde/HX711/blob/master/src/HX711.h
#include <HX711.h>
const int LOADCELL_DOUT_PIN = 32;
const int LOADCELL_SCK_PIN = 33;

HX711 scale;

void setup() {
  Serial.begin(9600);
  delay(10);
scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  
    }
void loop()
{

      
float pressure = (scale.read());
pressure = pressure - 2450000;     // add offset
pressure = pressure * (-0.000018); // add scale factor

delay(500); 
Serial.println(String(pressure));
}