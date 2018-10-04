
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

const char* ssid     = "SSID";
const char* password = "PASSWORD";
int16_t utc = -4;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;
// Time client using UDP
NTPClient timeClient(udp, "time.nist.gov", utc*3600, 60000);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.update();
}

int hour = 0;
int minute = 0;
int second = 99;

void loop() {
  if (second >= 60)
  {
    // Once a minute, ensure we're current with NTP time
    second = timeClient.getSeconds();
    minute = timeClient.getMinutes();
    hour = timeClient.getHours();
  }
  // Display the time
  Serial.print(hour);
  Serial.print(':');
  if (minute < 10) {
    Serial.print('0');
  }
  Serial.print(minute);
  Serial.print(':');
  if (second < 10) {
    Serial.print('0');
  }
  Serial.println(second);
  // wait one second
  delay(1000);
  // Update the time
  second = second + 1;
}
