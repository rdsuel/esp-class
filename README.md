# IoT with the ESP8266


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

## Exercise 5:
### Connecting to the Adafruit IO cloud platform.
Include the Adafruit MQTT libraries.  These allow us to send packets of data to the Adafruit IO cloud service.
```c
#include <ESP8266WiFi.h> // Should already have this from exercise 2.
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
