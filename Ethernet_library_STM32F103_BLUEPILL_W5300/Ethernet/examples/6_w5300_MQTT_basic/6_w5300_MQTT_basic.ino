/*
 Basic MQTT example

 This sketch demonstrates the basic capabilities of the library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic"
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 
*/

#include <Ethernet.h>
#include <PubSubClient.h>

#define LED PC13 // for RPi PICO onboard LED
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 5);
//IPAddress server(52,57,167,165);
IPAddress server(192, 168, 137, 1);
uint32_t premillis = 0;


EthernetClient ethClient;
PubSubClient client(ethClient);

void setup()
{
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  client.setServer(server, 1883);
  client.setCallback(callback);

  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  if(millis() - premillis > 8000)
  {
    premillis = millis();
    client.publish("outTopic","hello world");
  }
  delay(10);
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    if(payload[0] == '1') {digitalWrite(LED, HIGH);Serial.println("LED turned ON");client.publish("outTopic","LED turned ON");}
    else if(payload[0] == '0') {digitalWrite(LED, LOW);Serial.println("LED turned OFF");client.publish("outTopic","LED turned OFF");}
    else {Serial.println("Wrong command");}
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoclient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic","hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
