// /* Sweep
//  by BARRAGAN <http://barraganstudio.com>
//  This example code is in the public domain.

//  modified 8 Nov 2013
//  by Scott Fitzgerald
//  https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
// */

// #include <ESP32_Servo.h>

// Servo myservo;  // create servo object to control a servo
// // twelve servo objects can be created on most boards

// int pos = 0;    // variable to store the servo position

// void setup() {
//   myservo.attach(13);  // attaches the servo on pin 9 to the servo object
// }

// void loop() {
//   for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(50);                       // waits 15 ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(50);                       // waits 15 ms for the servo to reach the position
//   }
// }


#include <ESP32_Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "MEPPURATH";
const char* password = "06111960";

//Your Domain name with URL path or IP address with path
String serverName = "https://finance.yahoo.com/quote/TCS.NS?p=TCS.NS&.tsrc=fin-srch";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 1000;
Servo myservo;
int pos = 0;

void setup() {
  Serial.begin(9600); 
  myservo.attach(13);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // If you need Node-RED/server authentication, insert user and password below
      //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        // Serial.print("HTTP Response code: ");
        // Serial.println(httpResponseCode);
        String payload = http.getString();
        // Serial.println(payload);
               int startTagIndex = payload.indexOf("data-pricehint=\"2\" value=\"");
  int endTagIndex = payload.indexOf("\" active=");
  // Serial.println("======================================");
  // Serial.println(startTagIndex);
  // Serial.println(endTagIndex);
  // Serial.println("======================================");

  // Check if both start and end tags are found
  if (startTagIndex != -1 && endTagIndex != -1) {
    // Extract the data between the start and end tags
    String extractedData = payload.substring(startTagIndex+28, endTagIndex); // Add or subtract offsets as needed

    // Print the extracted data
    Serial.println(extractedData);
    float floatValue = extractedData.toFloat();
    int outputValue = map(floatValue, 70, 74, 0, 180);
    Serial.println(outputValue);
    myservo.write(outputValue);
  } else {
    Serial.print("Data not found");
  }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

