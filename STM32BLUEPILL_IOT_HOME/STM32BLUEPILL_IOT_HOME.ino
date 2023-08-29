// W5300 AND STM32F103 Based IoT Home automation
// Developed By - NK MAKER 
#include <Ethernet.h> // This library should be replaced with library available in project
#include <PubSubClient.h>
#include <Wire.h> 

// OLED libs
#include "SSD1306.h" 

#define LED    	PC13 // for STM32F103 onboard LED
#define RELAY1 	PB7
#define RELAY2 	PB6
#define RELAY3 	PB5
#define RELAY4 	PB3

#define DSPIN	PB4
// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 137, 5);

//IPAddress server(52,57,167,165);
char* server= "broker.hivemq.com";

uint32_t premillis = 0;
uint32_t premillis1 = 0;

uint8_t oled_flag1 = 10;
uint8_t oled_flag2 = 20;
uint8_t oled_flag3 = 30;
uint8_t oled_flag4 = 40;

char tempc[20] = {' '} ;
EthernetClient ethClient;
PubSubClient client(ethClient);

// Oled Display Instance
SSD1306  display(0x3C, PB9, PB8);


void setup()
{
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  pinMode(RELAY3,OUTPUT);
  pinMode(RELAY4,OUTPUT);
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);
  digitalWrite(RELAY3,HIGH);
  digitalWrite(RELAY4,HIGH);

  // Init OLED Sisplay
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 16, "W5300 IOT Hub");
  display.drawString(64, 32, "STM32F103");
  display.drawString(64, 48, "By NK Maker");
  display.display();
  delay(4000);

  client.setServer(server, 1883);
  client.setCallback(callback);

  Serial.println("** W5300 + STM32F103 BLUEPILL IOT HUB By - NK Maker **");
  Ethernet.begin(mac, ip);
  // Allow the hardware to sort itself out
  delay(1500);
}

void loop()
{
  if(millis() - premillis1 > 500)
  {
    premillis1 = millis();
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_LEFT);
    display.setFont(ArialMT_Plain_10);
    if(oled_flag1 == 10)display.drawString(0, 0,  "S1 OFF");
    if(oled_flag2 == 20)display.drawString(0, 16, "S2 OFF");
    if(oled_flag3 == 30)display.drawString(0, 32, "S3 OFF");
    if(oled_flag4 == 40)display.drawString(0, 48, "S4 OFF");
    if(oled_flag1 == 11)display.drawString(0, 0,  "S1 ON");
    if(oled_flag2 == 21)display.drawString(0, 16, "S2 ON");
    if(oled_flag3 == 31)display.drawString(0, 32, "S3 ON");
    if(oled_flag4 == 41)display.drawString(0, 48, "S4 ON");
    display.drawString(64, 0, "NW - YES");
    
    display.setFont(ArialMT_Plain_16);
    display.drawString(64, 24, " TEMP");
    display.drawString(64, 44, tempc);
    display.display();  
  }
  
  if (!client.connected()) {
    reconnect();
  }
  if(millis() - premillis > 4000)
  {
    premillis = millis();
    // put your main code here, to run repeatedly:
    double temp = TempRead();
    temp  = temp * 0.0625; // conversion accuracy is 0.0625 / LSB
    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    int val_int = (int)temp;   // compute the integer part of the float
    float val_float = (abs(temp) - abs(val_int)) * 100;
    int val_fra = (int)val_float;
    sprintf (tempc, "%d.%d", val_int, val_fra); 
    //Serial.println (tempc) ;
    client.publish("W5300_IOT_PUBT",tempc);
  }
  delay(10);
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    if(payload[0] == 'a') {digitalWrite(RELAY1, LOW);Serial.println("RELAY1 turned ON");client.publish("W5300_IOT_PUB","a");oled_flag1 = 11;}
    else if(payload[0] == 'b') {digitalWrite(RELAY2, LOW);Serial.println("RELAY2 turned ON");client.publish("W5300_IOT_PUB","b");oled_flag2 = 21;}
    else if(payload[0] == 'c') {digitalWrite(RELAY3, LOW);Serial.println("RELAY3 turned ON");client.publish("W5300_IOT_PUB","c");oled_flag3 = 31;}
    else if(payload[0] == 'd') {digitalWrite(RELAY4, LOW);Serial.println("RELAY4 turned ON");client.publish("W5300_IOT_PUB","d");oled_flag4 = 41;}
    else if(payload[0] == 'A') {digitalWrite(RELAY1, HIGH);Serial.println("RELAY1 turned OFF");client.publish("W5300_IOT_PUB","A");oled_flag1 = 10;}
    else if(payload[0] == 'B') {digitalWrite(RELAY2, HIGH);Serial.println("RELAY2 turned OFF");client.publish("W5300_IOT_PUB","B");oled_flag2 = 20;}
    else if(payload[0] == 'C') {digitalWrite(RELAY3, HIGH);Serial.println("RELAY3 turned OFF");client.publish("W5300_IOT_PUB","C");oled_flag3 = 30;}
    else if(payload[0] == 'D') {digitalWrite(RELAY4, HIGH);Serial.println("RELAY4 turned OFF");client.publish("W5300_IOT_PUB","D");oled_flag4 = 40;}
    else if(payload[0] == 'x') {digitalWrite(RELAY1, LOW);digitalWrite(RELAY2, LOW);digitalWrite(RELAY3, LOW);digitalWrite(RELAY4, LOW);Serial.println("ALL RELAY turned ON");client.publish("W5300_IOT_PUB","x");client.publish("W5300_IOT_PUB","a");client.publish("W5300_IOT_PUB","b");client.publish("W5300_IOT_PUB","c");client.publish("W5300_IOT_PUB","d");oled_flag1 = 11;oled_flag2 = 21;oled_flag3 = 31;oled_flag4 = 41;}
    else if(payload[0] == 'X') {digitalWrite(RELAY1, HIGH);digitalWrite(RELAY2, HIGH);digitalWrite(RELAY3, HIGH);digitalWrite(RELAY4, HIGH);Serial.println("ALL RELAY turned OFF");client.publish("W5300_IOT_PUB","X");client.publish("W5300_IOT_PUB","A");client.publish("W5300_IOT_PUB","B");client.publish("W5300_IOT_PUB","C");client.publish("W5300_IOT_PUB","D");oled_flag1 = 10;oled_flag2 = 20;oled_flag3 = 30;oled_flag4 = 40;}
    
    }
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoclient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("W5300_IOT_PUB","X");
      client.publish("W5300_IOT_PUB","A");
      client.publish("W5300_IOT_PUB","B");
      client.publish("W5300_IOT_PUB","C");
      client.publish("W5300_IOT_PUB","D");
      // ... and resubscribe
      client.subscribe("W5300_IOT_SUB");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

boolean DS18B20_Init()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(5);
  digitalWrite(DSPIN, LOW);
  delayMicroseconds(750);//480-960
  digitalWrite(DSPIN, HIGH);
  pinMode(DSPIN, INPUT);
  int t = 0;
  while (digitalRead(DSPIN))
  {
    t++;
    if (t > 60) return false;
    delayMicroseconds(1);
  }
  t = 480 - t;
  pinMode(DSPIN, OUTPUT);
  delayMicroseconds(t);
  digitalWrite(DSPIN, HIGH);
  return true;
}
 
void DS18B20_Write(byte data)
{
  pinMode(DSPIN, OUTPUT);
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(10);
    if (data & 1) digitalWrite(DSPIN, HIGH);
    else digitalWrite(DSPIN, LOW);
    data >>= 1;
    delayMicroseconds(50);
    digitalWrite(DSPIN, HIGH);
  }
}
 
byte DS18B20_Read()
{
  pinMode(DSPIN, OUTPUT);
  digitalWrite(DSPIN, HIGH);
  delayMicroseconds(2);
  byte data = 0;
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(1);
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);
    delayMicroseconds(5);
    data >>= 1;
    if (digitalRead(DSPIN)) data |= 0x80;
    delayMicroseconds(55);
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
  }
  return data;
}
 
int TempRead()
{
  if (!DS18B20_Init()) return 0;
  DS18B20_Write (0xCC); // Send skip ROM command
  DS18B20_Write (0x44); // Send reading start conversion command
  if (!DS18B20_Init()) return 0;
  DS18B20_Write (0xCC); // Send skip ROM command
  DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
  int temp = DS18B20_Read (); // Low byte
  temp |= DS18B20_Read () << 8; // High byte
  return temp;
}
