
// Written by Pete Sagat
// Date: 3/31/17
// Program: Sketch to gather humidity and temperature data via DHT22
// and post date via HTTP to an influxdb server.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#include <DHT.h> // DHT22 Sensor Library
#include <ESP8266WiFi.h> // Needed for ESP8266 to connect to WiFi
#include <WiFiClient.h> // 

#define DHTPIN 5     // what digital pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// Testing purposes without console. If esp cant connect to wifi or to influxdb a red led will light
int ledPinRed = 4;

const char* ssid     = "SSID";
const char* password = "SECRET";
const char* host = "x.x.x.x";
int port = 8086;

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
    digitalWrite(ledPinRed, LOW);
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  pinMode(ledPinRed, OUTPUT);
  digitalWrite(ledPinRed, LOW);
  Serial.begin(115200);
  setup_wifi();
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(f)) {
    digitalWrite(ledPinRed, HIGH); // Sets LED to on to indicate an issue
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  digitalWrite(ledPinRed, LOW); // Resets LED to OFF

  // Console output
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
    digitalWrite(ledPinRed, HIGH); // Sets LED to on to indicate an issue
    return;
  }
  digitalWrite(ledPinRed, LOW); // Resets LED to OFF
  // send the packet to influxdb
  String url = "/write?db=Custom";
  Serial.print("requesting URL: "); Serial.println(url);
  String body= "Humidity,location=Humidor value=" + String(h);
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Content-length: " + body.length() +"\r\n" 
               "Content-Type: application/x-www-form-urlencoded" +
               "Connection: close\r\n\r\n" +
               body
              );

// Lets sleep for 30 seconds
 delay(30000);

}
