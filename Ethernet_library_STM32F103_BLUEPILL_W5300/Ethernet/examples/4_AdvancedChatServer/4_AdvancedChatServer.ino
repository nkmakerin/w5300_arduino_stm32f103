#include <Arduino.h>
#include "Ethernet.h"


/* Network Info */
#define SERVER_PORT 5000
#define MAX_CLIENT  8

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 137, 5);
IPAddress myDns(8, 8, 8, 8);
IPAddress gateway(192, 168, 137, 1);
IPAddress subnet(255, 255, 255, 0);

EthernetServer server(SERVER_PORT);
EthernetClient clients[MAX_CLIENT];

void print_network_info(void);

void setup() {

  Serial.begin(115200);

  Ethernet.begin(mac, ip, myDns, gateway, subnet);

   print_network_info();
   server.begin();
}

void loop() {
  EthernetClient newClient = server.accept();

  if (newClient) {  	
    for (byte i=0; i < MAX_CLIENT; i++) {
      if (!clients[i]) {
        Serial.print("We have a new client # ");
        Serial.println(i);
        newClient.print("Hello, client number: ");
        newClient.print(i);
        newClient.print("\r\n");
        clients[i] = newClient;
        break;
      }
    }
  }

  for (byte i=0; i < MAX_CLIENT; i++) {
    if (clients[i] && clients[i].available() > 0) {
      byte buffer[80];
      int count = clients[i].read(buffer, 80);

      for (byte j=0; j < MAX_CLIENT; j++) {
        if (j != i && clients[j].connected()) {
          clients[j].write(buffer, count);
        }
      }
    }
  }

  for (byte i=0; i < MAX_CLIENT; i++) {
    if (clients[i] && !clients[i].connected()) {
      Serial.print("disconnect client # ");
      Serial.println(i);
      clients[i].stop();
    }
  }
}


void print_network_info(void){
  byte print_mac[6];
  Serial.println("\r\n-------------------------------------------------");
  Serial.print("MAC        : ");
  Ethernet.MACAddress(print_mac);
  for (byte i = 0; i < 6; i++) {
    Serial.print(print_mac[i], HEX);
    if (i < 5) {
      Serial.print(":");
    }
  }
  Serial.println();
  Serial.print("IP         : ");
  Serial.println(Ethernet.localIP());
  Serial.print("Gateway IP : ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("DNS        : ");
  Serial.println(Ethernet.dnsServerIP());
  Serial.println("-------------------------------------------------");
  Serial.print("Advanced Chat Server start ... on Port ");
  Serial.println( SERVER_PORT);
}
