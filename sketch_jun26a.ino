// Import required libraries
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
// defines pins numbers
const int trigPin = D4;  //D4 5
const int echoPin = D3;  //D3  4
// defines variables
long duration;
int distance;
const char* ssid = "ilelio";
const char* password = "lengo404as202";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String carslot() // function to get temperature from dht11
{
  float carstat = distance;

  return String(carstat);
}
String processor(const String& var)
{
  if (var == "DISTANCE")
  {
    return carslot();
  }
}
void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  // Initialize SPIFFS
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html", String(), false, processor);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", carslot().c_str());
  });
  // Start server
  server.begin();
}

void loop()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
 
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
 // delay(1000);
}
