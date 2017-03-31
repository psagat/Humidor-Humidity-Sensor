#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiUDP.h>
#include "DHT.h"
#include <WiFiClient.h>



// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain



#define DHTPIN 5     // what digital pin we're connected to

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
int ledPin = 2;

const char* ssid     = "Orion";
const char* password = "alphaomega";
//byte host[] = {192, 168, 1, 81};
const char* host = "192.168.1.81";
int port = 8086;
WiFiUDP udp;



WiFiClient espClient;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void setup() {
  Serial.begin(115200);
  setup_wifi();
  Serial.println("DHT22 test!");
  pinMode(ledPin, OUTPUT); 
  dht.begin();

  
}

void loop() {

  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(f)) {
    digitalWrite(ledPin, HIGH); // Sets LED to on to indicate an issue
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  digitalWrite(ledPin, LOW);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.println();



// Use WiFiClient to connect to influxdb
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
  
  // send the packet to influxdb
  String url = "/write?db=Custom";
  Serial.print("requesting URL: "); Serial.println(url);
String body= "Humidity,key=Humidor value= " + String(h);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Content-length: " + body.length() +"\r\n" 
               "Content-Type: application/x-www-form-urlencoded" +
               "Connection: close\r\n\r\n" +
               body
              );

 delay(30000);

  

}
