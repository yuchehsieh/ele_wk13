#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
//溫濕度感測Library
#include <dht.h>

String apiKey = "QKM0HPXD90ZT4GTU";//Channel Write API Key 
const char* ssid = "iPhone";//AP ID
const char* password = "00000000";//AP 密碼

const char* server = "api.thingspeak.com";
#define DHTPIN D2 //溫濕度感測器的輸出角位

dht DHT;
WiFiClient client;

void setup() {
Serial.begin(115200);
delay(10);
WiFi.begin(ssid, password);
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected"); 
} 
void loop() {
DHT.read11(DHTPIN);
float h = DHT.humidity;
float t = DHT.temperature;
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");//使用POST傳送資料
client.print("Host: api.thingspeak.com\n");//Host位置
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");//API KEY
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());//POST資料長度
client.print("\n\n");
client.print(postStr);//POST資料內容

Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius Humidity: ");
Serial.print(h);
Serial.println("% send to Thingspeak");
}
client.stop(); 
Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(20000);
}

