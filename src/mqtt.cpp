#include <Arduino.h>

// https://pubsubclient.knolleary.net/api.html
#include <PubSubClient.h>

const char *mqtt_client_id = "ESP32Garden";
const char *mqtt_server = "farmer.cloudmqtt.com";
const int mqtt_port = 18647;
const char *mqtt_user = "jnwomnzt";
const char *mqtt_password = "gOcaxgQSVTuA";

class mqtt
{
public:
    // This functions reconnects your ESP to your MQTT broker
    // Change the function below if you want to subscribe to more topics with your ESP8266
    static void reconnect()
    {
        // Loop until we're reconnected
        while (!client.connected())
        {
            Serial.println("Attempting MQTT connection...");
            // Attempt to connect
            if (client.connect(mqtt_client_id, mqtt_user, mqtt_password))
            {
                Serial.println("connected");
                // Subscribe or resubscribe to a topic
                // You can subscribe to more topics (to control more LEDs in this example)
                client.subscribe("home/garden");
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }
};