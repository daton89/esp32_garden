#ifndef mqtt_h
#define mqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class Mqtt
{
private:
    WiFiClient espClient;
    PubSubClient client;
    char *mqtt_client_id;
    char *mqtt_server;
    int mqtt_port;
    char *mqtt_user;
    char *mqtt_password;
    int lamp;

public:
    Mqtt();
    void setup();
    void connect();
    void publish(char *channel, char *message);
    static void callback(String topic, byte *message, unsigned int length);
};

#endif /* _MQTT_H_ */