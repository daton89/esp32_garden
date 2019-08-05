#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClient.h>
// https://pubsubclient.knolleary.net/api.html
#include <PubSubClient.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include <sleep.cpp>
#include <mqtt.cpp>

// Comment this out to disable prints and save space
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

const char *ssid = "Batcave";
const char *password = "iambrucewayne";

const char *mqtt_client_id = "ESP32Garden";
const char *mqtt_server = "farmer.cloudmqtt.com";
const int mqtt_port = 18647;
const char *mqtt_user = "jnwomnzt";
const char *mqtt_password = "gOcaxgQSVTuA";

// Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
const int lamp = 4;

WiFiClient espClient;
PubSubClient client(espClient);

RTC_DATA_ATTR int bootCount = 0;

void setup_wifi()
{
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
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Here we can add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
  if (topic == "home/garden/light")
  {
    Serial.print("Changing garden/light to ");
    if (messageTemp == "on")
    {
      digitalWrite(lamp, HIGH);
      Serial.print("On");
    }
    else if (messageTemp == "off")
    {
      digitalWrite(lamp, LOW);
      Serial.print("Off");
    }
  }
  Serial.println();
}


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
  client.publish("home/garden/temperature", temperatureTemp);
  client.publish("home/garden/humidity", humidityTemp);

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

  // Increment boot number and print it every reboot
  ++bootCount;
  Serial.println("Boot number: " + String(bootCount));

  // Print the wakeup reason for ESP32
  sleep::print_wakeup_reason();

  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(callback);

  if (!client.connected())
  {
    Serial.println("MQTT Client reconnect...");
    reconnect();
  }
  if (!client.loop())
  {
    Serial.println("MQTT Client not in loop...");
    client.connect(mqtt_client_id, mqtt_user, mqtt_password);
  }

  publish_values();

  // Go to sleep for 30 seconds
  sleep::go_to_sleep();
}

void loop() {}