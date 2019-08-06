#include <mqtt.h>

#include <Arduino.h>

Mqtt::Mqtt()
{
  client = PubSubClient(espClient);
  mqtt_client_id = "ESP32Garden";
  mqtt_server = "farmer.cloudmqtt.com";
  mqtt_port = 18647;
  mqtt_user = "jnwomnzt";
  mqtt_password = "gOcaxgQSVTuA";
  // Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
  lamp = 4;
}

void Mqtt::setup()
{
  client.setServer(mqtt_server, mqtt_port);

  client.setCallback(Mqtt::callback);
}

// This functions reconnects your ESP to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP
void Mqtt::connect()
{
  // if (!client.connected())
  // {
  //     mqtt::connect();
  // }
  // if (!client.loop())
  // {
  //     client.connect(mqtt_client_id, mqtt_user, mqtt_password);
  // }

  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqtt_client_id, mqtt_user, mqtt_password))
    {
      Serial.println("MQTT client connected!");
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

void Mqtt::publish(char *channel, char *message)
{
  client.publish(channel, message);
}

// This functions is executed when some device publishes a message to a topic that your ESP is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that
// your ESP is subscribed you can actually do something
void Mqtt::callback(String topic, byte *message, unsigned int length)
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

  // If a message is received on the topic home/garden/light,
  // check if the message is either on or off.
  // Turns the lamp GPIO according to the message
  if (topic == "home/garden/light")
  {
    Serial.print("Changing garden/light to ");
    if (messageTemp == "on")
    {
      digitalWrite(4, HIGH);
      Serial.print("On");
    }
    else if (messageTemp == "off")
    {
      digitalWrite(4, LOW);
      Serial.print("Off");
    }
  }
  Serial.println();
}
