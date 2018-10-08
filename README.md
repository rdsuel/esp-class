# IoT with the ESP8266
The documentation below provides snippets of code that we will use for the various class exercises.  These are provided to simplify the coding process for participants that may be less comfortable with coding.

The additional board manager URL is:
https://adafruit.github.io/arduino-board-index/package_adafruit_index.json,http://arduino.esp8266.com/stable/package_esp8266com_index.json

## Exercise 1:
### Scheduler
The following code provides a basic architecture for running periodic "tasks".  In this example, the built-in ESP8266 LED toggles from a callback function (task) every 500 ms.
```c
void setup()
{
  // Exercise 1: Blink the on-board LED.
  pinMode(LED_BUILTIN, OUTPUT);
}

void LedTask()
{
  int state = digitalRead(LED_BUILTIN);  
  digitalWrite(LED_BUILTIN, !state);   
}

typedef struct
{
  unsigned long previousMillis;
  unsigned long elapsedMillis;
  unsigned long timeoutMillis;
  void (*callback)();
} Timer_t;

static Timer_t schedulerTable[] =
{
  {0, 0, 500, &LedTask},
  // Add tasks here.
};

void runScheduler()
{
  // Run each timer in the scheduler table, and call
  for (int i = 0; i < sizeof(schedulerTable)/sizeof(Timer_t); i++)
  {
    // Note: millis() will overflow after ~50 days.  
    unsigned long currentMillis = millis();
    Timer_t *t = &schedulerTable[i];    
    t->elapsedMillis += currentMillis - t->previousMillis;
    t->previousMillis = currentMillis;
    if (t->elapsedMillis >= t->timeoutMillis)
    {
      t->elapsedMillis = 0;
      t->callback();
    }
  }
}

void loop()
{  
  runScheduler();
}
```


## Exercise 2:
### Connecting to WiFi.
First, you need to pull in the ESP8266 Wifi library.
```c
#include <ESP8266WiFi.h>
```

Next, add the `ssid` and `password` variables that we will use to connect to the network. Change to the correct values.
```c
const char* ssid     = "SSID";
const char* password = "PASSWORD";
```

Finally, in setup, connect to the network ...
```c
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
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
```


## Exercise 3:
### Driving an OLED display using NTP time
The following includes will be needed to drive the display, and to read the current time from the online NTP (network time protocol) server.
```c
#include <Wire.h>
#include "SSD1306Wire.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
```

The following variables will support an I2C display connected to pins GPIO5 (SDA) and GPIO5 (SCK) with an address of 0x3C.
```c
SSD1306Wire  display(0x3c, 5, 4);
```

These variables will connect to the `time.nist.gov` server and offset the time for Eastern Daylight Time (Louisville, KY).
```c
int utc = -4; // Eastern daylight time
WiFiUDP udp;
NTPClient timeClient(udp, "time.nist.gov", utc * 3600, 60000);
```

Next we need to initialize the display and the NTP client in `setup()`.  This snippet initializes the display object we created, and will print "Hello world" to the display in 24 point Arial font.
```c
  display.init();
  display.clear();
  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hello world");
  display.display();
```

This snippit will start the NTP client and pull the current time.
```c
  timeClient.begin();
  timeClient.update();
```

We can write text to the display using the following process.
```c
  display.clear();    // Clear screen.
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_24);
  display.drawString(63, 0, timeOfDay);
  display.display(); // Render screen.
```

We can read the NTP time using the following code.
```c
timeClient.update();
String timeOfDay = timeClient.getFormattedTime();
Serial.println(timeOfDay);
```

## Exercise 4:
### Reading the DHT22 temperature and humidity sensor.
Include the DHT libraries.  Thes allow us to easily read the sensor.
```c
#include <Adafruit_Sensor.h>
#include <DHT.h>
```

Create variables for the DHT sensor object and also for the temperature and humidity values.  This example assumes a DHT22 sensor type connected to the GPIO0 pin on the ESP8266.
```c
DHT dht(0, DHT22);
float temperature;
float humidity;
```

Initialize the sensor object in `setup()`.
```c
  dht.begin();
```

The read it periodically. It is recommended not to read the sensor faster than every 2 seconds.
```c
  humidity = dht.readHumidity();
  // This reads the sensor in degrees F.
  temperature = dht.readTemperature(true);
  // This reads the sensor in degrees C.
  temperature = dht.readTemperature();
```


## Exercise 5:
### Connecting to the Adafruit IO cloud platform.
Include the Adafruit MQTT libraries.  These allow us to send packets of data to the Adafruit IO cloud service.
```c
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
```

Add the following defines and fill in your username and key from the Adafruit IO system.
```c
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "YOUR AIO USERNAME"
#define AIO_KEY         "YOUR AIO KEY"
```

Add the following variables, which point to your temperature and humidity feeds in Adafruit IO.
```c
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish temperatureFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");
Adafruit_MQTT_Publish humidityFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/humidity");
```

Finally, connect to the Adafruit IO system with the following function.
```c
void connectToAdafruit() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0)
  {
    switch (ret)
    {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
    {
      mqtt.disconnect();
    }

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }

  Serial.println(F("Adafruit IO Connected!"));
}
```
