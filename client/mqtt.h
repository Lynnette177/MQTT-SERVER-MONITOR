#include "weather.h"
#include <PubSubClient.h>
const int mqttPort = 1883;
const char* mqttServer = "";
const char* mqttUser = "";
const char* mqttPassword = "";
const char* mqttTopic = "";
WiFiClient espClient;
PubSubClient client(espClient);
String mqtt_msg = "";
DynamicJsonDocument stat1(1024);

float cpupercent = 0;
int cfreq = 0;
int minfreq = 0;
int maxfreq = 0;
float memtotal = 0;
float memava = 0;
float mempercent = 0;
float memused = 0;
float memfree = 0;
float swaptotal = 0;
float swapused = 0;
float swapfree = 0;
float swappercent = 0;
unsigned int uptime = 0;
float cputemp = 0;
int alert = 0;
String time_str="";

void generate_time_str(){
  if (uptime > 86400){
    int day = uptime/86400;
    int hour = (uptime % 86400)/3600;
    time_str = String(day) + "d" + String(hour) + "h";
  }
  else{
    int hour = (uptime / 3600);
    int min = (uptime % 3600)/60;
    time_str = String(hour) + "h" + String(min) + "m";
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  DynamicJsonDocument stat1(1024);
  String payloadStr = "";
  for (int i = 0; i < length; i++) {
    payloadStr += (char)payload[i];
  }
  mqtt_msg = payloadStr;
  //Serial.print("Received payload: ");
  //Serial.println(payloadStr);
  deserializeJson(stat1, mqtt_msg);
  cpupercent = stat1["cpu_percent"].as<float>();
  cfreq = stat1["cpu_freq"]["current"].as<int>();
  minfreq = stat1["cpu_freq"]["min"].as<int>();
  maxfreq = stat1["cpu_freq"]["max"].as<int>();
  memtotal = stat1["virtual_memory"]["total"].as<float>();
  memava = stat1["virtual_memory"]["available"].as<float>();
  mempercent = stat1["virtual_memory"]["percent"].as<float>();
  memused = stat1["virtual_memory"]["used"].as<float>();
  memfree = stat1["virtual_memory"]["free"].as<float>();
  swaptotal = stat1["swap_memory"]["total"].as<float>();
  swapused = stat1["swap_memory"]["used"].as<float>();
  swapfree = stat1["swap_memory"]["free"].as<float>();
  swappercent = stat1["swap_memory"]["percent"].as<float>();
  uptime = stat1["up_time"].as<unsigned int>();
  cputemp = stat1["cpu_temperature"].as<float>();
  alert = stat1["alert"].as<int>();
  generate_time_str();
  //Serial.printf("%f %d %d %d %f %f %f %f %f %f %f %f %f %d %f %d\n",cpupercent,cfreq,minfreq,maxfreq,memtotal,memava,mempercent,memused,memfree,swaptotal,swapused,swapfree,swappercent,uptime,cputemp,alert);
}
void init_mqtt(){
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Connecting to MQTT Broker...");
    if (client.connect("ESP32Client", mqttUser, mqttPassword)) {
      Serial.println("Connected to MQTT Broker");
      client.subscribe(mqttTopic);
    } else {
      Serial.print("Failed to connect to MQTT Broker. Retrying in 5 seconds...");
      delay(5000);
    }
  }
}