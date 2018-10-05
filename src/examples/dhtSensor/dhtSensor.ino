
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define DHTPIN  4         // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

//Variables
float humidity;
float temperature;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Setup start");
  dht.begin();
  Serial.println("Setup done");
}

void loop() {
  // put your main code here, to run repeatedly:
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  //Print temp and humidity values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Temp: ");
  Serial.print(temperature);
  Serial.println(" Celsius");
  delay(2000);
}
