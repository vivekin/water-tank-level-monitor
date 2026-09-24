    /////////////////////////////////////////////////////////////////
   //         ESP32 Web Server Project  v1.00                     //
  //       Get the latest version of the code here:              //
 //         http://educ8s.tv/esp32-web-server                   //
/////////////////////////////////////////////////////////////////


#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>  //https://github.com/bbx10/WebServer_tng

WebServer server ( 80 );

const char* ssid     = "MEPPURATH";
const char* password = "06111960";

  //If the sensor does not work, try the 0x77 address as well
// int LEDPIN = 2;
// String  ledState = "OFF";

const int trigPin = 5;
const int echoPin = 18;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

long duration;
float distanceCm;
float y;


void setup() 
{
  // pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(115200);



  connectToWifi();

  beginServer();
// Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {
 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED/2;
  
  
  // Prints the distance in the Serial Monitor
  Serial.print("Distance to surface (cm): ");
  Serial.println(distanceCm);
  y = map(distanceCm, 8, 113, 100, 1);
  Serial.print("Tank %: ");
  Serial.println(y);
delay(2000);
 server.handleClient();
 delay(2000);
 
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  
  delay(2000);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(1000);
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
  delay(1000);
}

void handleRoot(){ 
  if ( server.hasArg("LED") ) {
    // handleSubmit();
    server.send ( 200, "text/html", getPage() );
  } else {
    server.send ( 200, "text/html", getPage() );
  }  
}


// void handleSubmit() {

//   String LEDValue;
//   LEDValue = server.arg("LED");
//   Serial.println("Set GPIO "); 
//   Serial.print(LEDValue);
  
//   if ( LEDValue == "1" ) {
//     digitalWrite(LEDPIN, HIGH);
//     ledState = "On";
//     server.send ( 200, "text/html", getPage() );
//   }
//   else if( LEDValue == "0" ) 
//   {
//     digitalWrite(LEDPIN, LOW);
//     ledState = "Off";
//     server.send ( 200, "text/html", getPage() );
//   } else 
//   {
//     Serial.println("Error Led Value");
//   }
// }

String getPage(){
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer</title>";
  page += "<style> body { background-color: #040404; font-family: Arial, Helvetica, Sans-Serif; Color: #53CBFB; font-size: 7vw; }</style>";
  // page += "<style> body { background-color: #f0f0f0; font-family: 'Roboto', Arial, Helvetica, Sans-Serif; color: #1a1a1a; margin: 20px; }</style>";
  page += "</head><body><h1>ESP32 Water Tank Monitor</h1>";
  page += "<h3>Distance to surface : "+String(distanceCm)+" cm</h3>";
  page += "<h2>Tank : "+String(y)+"% </h2>";

  page += "</body></html>";
  return page;
}
