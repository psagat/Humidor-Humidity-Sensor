# Humidor Humidity Sensor

Humidity Sensor that takes readings every 30 seconds and posts the data to an influxdb server to charting.


<a href="http://imgur.com/EKuIALh"><img src="http://i.imgur.com/EKuIALh.png" title="source: imgur.com" /></a>

## Getting Started

These instructions will provide you with the breadboard schematic, the sketch, and the changes within the sketch you will need to make.
It will NOT go into configuring grafana or influxdb as there are plenty instructions found with a simple google search.

### Prerequisites

You will need the following environement setup:

#### Software:
1.Grafana or other graphing utility

2.Influxdb

3. Arduino IDE (DHT,ESP8266WiFi, and WiFiClient libraries)

#### Hardware:

1.nodemcu ESP8266 Dev board

2.DHT22 Temp and Humidity Sensor

#### Breadboard Schematic
<a href="http://imgur.com/mi70bTi"><img src="http://i.imgur.com/mi70bTi.png" title="source: imgur.com" /></a>

### Configure Sketch

Download These changes need to be made to the sketch based on your own environment.

#### Configure network information

```
const char* ssid     = "SSID"; ------- Change to your SSID
const char* password = "SECRET"; ------ Your PW
const char* host = "x.x.x.x"; ------ IP address of your influxdb server
```
#### Configure POST HTTP request

```
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
```
```
  String url = "/write?db=Custom"; <----------- Change db= to the database you want to send the data to (db=mydb)
```
```
  String body= "Humidity,location=Humidor value=" + String(h); <------ You can change the name and the location of the dataset
```

When configured correctly you should see the following output in your console (ctrl-shift-M)

<a href="http://imgur.com/db3Rcvc"><img src="http://i.imgur.com/db3Rcvc.png" title="source: imgur.com" /></a>

### Configure Grafana Metrics

Your graph should look like this:

<a href="http://imgur.com/Y0EdoW7"><img src="http://i.imgur.com/Y0EdoW7.png" title="source: imgur.com" /></a>

"Humidity" and "Location" can be change to whatever you want when you configure the HTTP Post section of the sketch.

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

### And coding style tests

Explain what these tests test and why

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Dropwizard](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [Maven](https://maven.apache.org/) - Dependency Management
* [ROME](https://rometools.github.io/rome/) - Used to generate RSS Feeds

## Contributing

Please read [CONTRIBUTING.md](https://gist.github.com/PurpleBooth/b24679402957c63ec426) for details on our code of conduct, and the process for submitting pull requests to us.

## Versioning

We use [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/your/project/tags). 

## Authors

* **Billie Thompson** - *Initial work* - [PurpleBooth](https://github.com/PurpleBooth)

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Hat tip to anyone who's code was used
* Inspiration
* etc
