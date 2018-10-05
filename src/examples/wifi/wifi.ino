
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "iot_class";
const char* password = "password";

const int timeZone = 4;

// local port to listen for UDP packets
unsigned int localPort = 2390;
// time.nist.gov NTP server address
IPAddress timeServerIP;
const char* ntpServerName = "time.nist.gov";
// NTP time stamp is in the first 48 bytes of the message
const int NTP_PACKET_SIZE = 48;
//buffer to hold incoming and outgoing packets
byte packetBuffer[ NTP_PACKET_SIZE];
// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

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
  udp.begin(localPort);
  Serial.println(udp.localPort());
}

int hour = 0;
int minute = 0;
int second = 0;

void loop() {
  if (0==second)
  {
    // Once a minute, ping the NTP server
    WiFi.hostByName(ntpServerName, timeServerIP);
    sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  }
  // Check for a response
  int cb = udp.parsePacket();
  if (cb) {
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    // now convert NTP time into everyday time:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print the hour, minute and second:
    hour = (epoch  % 86400L) / 3600;
    minute = (epoch % 3600) / 60;
    second = epoch % 60;
    // Sort the timezone out
    hour = (hour + 24 - timeZone) % 24;
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
  if (60==second){
    second=0;
    minute++;
  }
  if (60==minute){
    minute=0;
    hour = (hour + 1) % 24;
  }
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress& address) {
  Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
