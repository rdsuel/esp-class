
#include <ESP8266WiFi.h>
#include <DHT.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "ADAFRUIT USERNAME"
#define AIO_KEY         "ADAFRUIT KEY"

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup feeds for temperature & humidity
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");

#define DHTPIN  4         // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// WiFi parameters
#define WLAN_SSID       "SSID"
#define WLAN_PASS       "PASSWORD"

float temperature;
float humidity;

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));
}

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println("DHT connecting ");
  dht.begin();

  Serial.println("WiFi connecting ");
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature() * 9.0f/5.0f + 32.0;

  if(! mqtt.ping(3))
  {
    if(! mqtt.connected())
    {
      connect();
    }
  }

  if (! temperatureFeed.publish(temperature)) 
  {
    Serial.println(F("Failed to publish temperature"));
  }
  else
  {
    Serial.println(F("Temperature published!"));
  }
 
  if (! humidityFeed.publish(humidity))
  {
    Serial.println(F("Failed to publish humidity"));
  }
  else
  {
    Serial.println(F("Humidity published!"));
  }
  delay(10000);
}
