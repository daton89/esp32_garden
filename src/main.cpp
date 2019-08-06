#include <Arduino.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <sleep.h>
#include <mqtt.h>
#include <wifi.cpp>

// Comment this out to disable prints and save space
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

Mqtt mqtt;

RTC_DATA_ATTR int bootCount = 0;

// float temperature, humidity, pressure, altitude;

void publish_values()
{
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = bme.readHumidity();
  // Read temperature as Celsius (the default)
  float t = bme.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from BME sensor!");
    return;
  }

  // Computes temperature values in Celsius
  static char temperatureTemp[7];
  dtostrf(t, 6, 2, temperatureTemp);

  static char humidityTemp[7];
  dtostrf(h, 6, 2, humidityTemp);

  // Publishes Temperature and Humidity values
  mqtt.publish("home/garden/temperature", temperatureTemp);
  mqtt.publish("home/garden/humidity", humidityTemp);

  // Log values to serial monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t Temperature: ");
  Serial.print(t);
  Serial.println(" *C ");
}

void setup()
{
  Serial.begin(9600);
  bme.begin(0x76);

  // Take some time to open up the Serial Monitor
  delay(1000);

  // Print the wakeup reason for ESP32
  Sleep::print_wakeup_reason(bootCount);

  wifi::connect();

  mqtt.setup();

  mqtt.connect();

  publish_values();

  // Go to sleep for 30 seconds
  Sleep::go_to_sleep();
}

void loop() {}