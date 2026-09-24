/////////////////////////////////////////////////////////////////
//         ESP32 Web Server Project  v1.00                     //
//       Get the latest version of the code here:              //
//         http://educ8s.tv/esp32-web-server                   //
/////////////////////////////////////////////////////////////////


#include <WiFi.h>
#include <WebServer.h>  //https://github.com/bbx10/WebServer_tng

WebServer server ( 80 );

const char* ssid     = "MEPPURATH";
const char* password = "06111960";

IPAddress staticIP(192, 168, 1, 4);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns1(103, 160, 195, 230);
IPAddress dns2(103, 199, 160, 80);



//https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home

int trigPin = 5;   // Trigger
int echoPin = 18;  // Echo
long duration, cm;
#define WINDOW_SIZE 5
float y;

int INDEX = 0;
int VALUE = 0;
int SUM = 0;
int READINGS[WINDOW_SIZE];
int AVERAGED = 0;

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#define LEDPIN 4
#define TANKLED 2
int brightness = 0;


void setup()
{
  pinMode(LEDPIN, OUTPUT);
  pinMode(TANKLED, OUTPUT);


  Serial.begin(115200);

  connectToWifi();

  beginServer();
  // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}

void loop() {

  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(LEDPIN, HIGH);
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
  y = map(AVERAGED, 8, 113, 100, 1);
  brightness = map(y, 10, 105, 20, 230);
  analogWrite(TANKLED, brightness);
  delay(500);
  server.handleClient();
  digitalWrite(LEDPIN, LOW);
  delay(2000);
}

void connectToWifi()
{
  WiFi.enableSTA(true);
  if (WiFi.config(staticIP, gateway, subnet, dns1, dns2) == false) {
    Serial.println("Configuration failed.");
  }

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
  Serial.print("Subnet Mask: " );
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));
  delay(1000);
}

void beginServer()
{
  server.on ( "/", handleRoot );
  server.begin();
  Serial.println ( "HTTP server started" );
  delay(1000);
}

void handleRoot() {
  server.send ( 200, "text/html", getPage() );
}


String getPage() {
  String page = "<html lang=en-EN><head><meta http-equiv='refresh' content='60'/>";
  page += "<title>ESP32 WebServer</title>";
  page += "<style> body { background-color: #040404; font-family: Arial, Helvetica, Sans-Serif; Color: #53CBFB; font-size: 7vw; }</style>";
  // page += "<style> body { background-color: #f0f0f0; font-family: 'Roboto', Arial, Helvetica, Sans-Serif; color: #1a1a1a; margin: 20px; }</style>";
  page += "</head><body><h1>ESP32 Water Tank Monitor</h1>";
  page += "<h3>Distance to surface : " + String(AVERAGED) + " cm</h3>";
  page += "<h2>Tank : " + String(y) + "% </h2>";

  page += "</body></html>";
  return page;
}
