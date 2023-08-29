#include <Arduino.h>
#include "Ethernet.h"

/* Network */
#define SERVER_PORT 5000
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 5); 

EthernetServer server(SERVER_PORT);
void print_network_info(void);

int loop_count = 0;
void setup() {

  Serial.begin(115200);

  Ethernet.begin(mac, ip);
  print_network_info();
  
  // start the server
  server.begin();
  Serial.print("server is at http://");
  Serial.print(Ethernet.localIP());
  Serial.print(":");
  Serial.println(SERVER_PORT);
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
  
          if (c == '\n') {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close"); 
          client.println("Refresh: 5");  
          client.println();
          client.println("<!DOCTYPE HTML>");

          client.println("<html>");
          client.print("Hello World from W5300 ** ");
          client.print("Counter = ");
          client.print(loop_count/2);
          client.print("\r\n");
          client.println("</html>");
          loop_count++;
          break;
        }
      }
    }

    delay(1);
    client.stop();
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
  Serial.print("IP : ");
  Serial.print(Ethernet.localIP());
  Serial.print("   Port : ");
  Serial.println(SERVER_PORT);
  Serial.println("-------------------------------------------------");
}
