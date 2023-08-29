#include <Ethernet.h>
#include <EthernetUdp.h>


/* Network */
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 137, 5);
unsigned int localPort = 1990;      // local port to listen on

char packetBuffer[24];  // buffer to hold incoming packet,
EthernetUDP Udp;
void print_network_info(void);

void setup() {
  
  Serial.begin(115200);
  delay(1000);
  Ethernet.begin(mac, ip);
  delay(1000);
  // start UDP
  print_network_info();
  Udp.begin(localPort);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  
    if (packetSize) {

    Serial.print("\r\n[From ");
    Serial.print(Udp.remoteIP());
    Serial.print(": ");
    Serial.print(Udp.remotePort());
    Serial.println("]:");
    // read the packet into packetBufffer
    Udp.read(packetBuffer, 24);
    Serial.println(packetBuffer);
    delay(10);
    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(packetBuffer);
    Udp.endPacket();
  }
  delay(100);
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
  Serial.print(Ethernet.localIP());
  Serial.print(": ");
  Serial.println(localPort);
  Serial.println("-------------------------------------------------");
}
