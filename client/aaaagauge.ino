#include <TFT_eSPI.h>
#include "mqtt.h"
#include"olight16.h"
#define PIN_POWER_ON 15
#define backColor 0x03EF
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite = TFT_eSprite(&tft);

int drawfreq = 600;
float drawtmp = 20;
unsigned int t = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    strcpy(timeHour,"00");
    strcpy(timeMin,"00");
    strcpy(timeSec,"00");
    strcpy(timeWeekDay,"00");
    strcpy(day,"00");
    strcpy(month,"00");
    strcpy(year,"00");
    return;
  }
  strftime(timeHour,3, "%H", &timeinfo);
  strftime(timeMin,3, "%M", &timeinfo);
  strftime(timeSec,3, "%S", &timeinfo);
  strftime(timeWeekDay,10, "%A", &timeinfo);
  strftime(day,3, "%d", &timeinfo);
  strftime(month,6, "%B", &timeinfo);
  strftime(year,5, "%Y", &timeinfo);
}

void get_weather(void * pvParameters){
  while(1){
    ParseWeather();
    while (Weather=="0")ParseWeather();
    delay(1200000);
  }
}

void updateArcs(float volt,int fps) {
//  if(drawfreq < cfreq-(cfreq%10)) drawfreq+=10;
//  else if (drawfreq > cfreq-(cfreq%10)) drawfreq-=10;
//  if (drawtmp < (int)cputemp) drawtmp+=0.5;
//  else if (drawtmp > (int)cputemp+1) drawtmp -=0.5;
  if (drawfreq != cfreq){
    if (cfreq-drawfreq >5 || cfreq-drawfreq < - 5){
      float drawdiff = ((float)(cfreq-drawfreq))/20.0f;
      if (drawdiff < 5 && drawdiff > 0) drawdiff = 5;
      else if (drawdiff > -5 && drawdiff < 0) drawdiff = -5;
      drawfreq += drawdiff;
    }
    else drawfreq += cfreq-drawfreq;
  }
  if (drawtmp != cputemp) drawtmp +=(cputemp - drawtmp)/10;
  sprite.setFreeFont(&Orbitron_Light_24);
  sprite.fillSprite(backColor);
  // Draw left bottom arc with custom percentage
  int leftBottomX = 60;
  int leftBottomY = 110;
  int arcRadius = 50;
  int arciRadius = 45;
  int startAngle = 50;
  int endAngle = map(drawtmp, 0, 100, 50, 310);
  uint16_t arcColor = TFT_GREEN;
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius,arciRadius, endAngle, 310, TFT_WHITE, backColor);
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius,arciRadius, startAngle, endAngle, arcColor, backColor);
  // Draw another left bottom arc with a different color and percentage
  int arcRadius2 = 38;
  int arciRadius2 = 33;
  int endAngle2 = map(drawfreq, minfreq, maxfreq, 50, 310);
  uint16_t arcColor2 = TFT_BLUE;
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius2,arciRadius2, endAngle2, 310, TFT_WHITE, backColor);
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius2,arciRadius2,startAngle, endAngle2, arcColor2, backColor);

  leftBottomX = 170;
  leftBottomY = 110;
  int startAngle3 = 50;
  int endAngle3 = map(memused, 0, memtotal, 50, 310);
  uint16_t arcColor3 = TFT_MAGENTA;
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius,arciRadius, endAngle3, 310, TFT_WHITE, backColor);
  int endAngleC = map((memtotal-memfree),0,memtotal,50,310);
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius,arciRadius, endAngle3, endAngleC, TFT_VIOLET, backColor);
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius,arciRadius, startAngle3, endAngle3, arcColor3, backColor);
  // Draw another left bottom arc with a different color and percentage
  int endAngle4 = map(swapfree, 0, swaptotal, 310, 50);
  uint16_t arcColor4 = TFT_RED ;
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius2,arciRadius2, endAngle4, 310, TFT_GREENYELLOW, backColor);
  sprite.drawArc(leftBottomX, leftBottomY, arcRadius2,arciRadius2,startAngle3, endAngle4, TFT_RED, backColor);

  sprite.setTextColor(TFT_WHITE);
  // Draw text on the top and right side
  sprite.setTextSize(1);
  sprite.drawString("CPU", 60, 150);
  sprite.drawString("MEM", 170, 150);
  sprite.drawString("UPTIME", 270, 75);
  sprite.drawString(time_str, 270, 100);
  sprite.drawString("BATT", 270, 125);
  String timestring = String(timeHour)+":"+String(timeMin);
  int timeWidth = sprite.textWidth(timestring);
  sprite.drawString(String(timeHour)+":"+String(timeMin), 280-(timeWidth/4), 45);

  sprite.setTextColor(TFT_PINK);
  sprite.setFreeFont(&Roboto_Light_12);
  sprite.setTextSize(2);
  sprite.setTextSize(1);
  sprite.drawString(String(cpupercent)+"%", 60, 100);
  sprite.drawString("T:" + String(cputemp), 60, 115);

  sprite.drawString(String(mempercent)+"%", 170, 100);
  sprite.drawString("Cached:", 170, 115);
  sprite.drawString(String((memtotal-memused-memfree)/memtotal*100)+"%", 170, 130);
  sprite.drawString("Weather:" + Weather, 60, 9);
  sprite.drawString("Temperature:" + temperature, 60, 23);
  sprite.drawString("Updated:"+lastupdatetimepart, 60, 37);
  sprite.drawString("IP:" + IP, 60, 51);


  sprite.drawString("Total:" + String(memtotal), 170, 9);
  sprite.drawString(String(memused)+ "MB", 170, 23);
  sprite.drawString(String(memava)+ "MB", 170, 37);
  sprite.drawString("Swap:" + String(swappercent) + "%", 170, 51);
  
  sprite.drawString(String(month)+"/"+String(year),270,12);
  sprite.drawString(String(timeWeekDay)+"/"+String(day),270,27);
  sprite.setTextColor(TFT_WHITE);
  sprite.drawString(String(timeSec), 284+(timeWidth/3), 46);
  sprite.drawString(String(float(volt),2) + "V",250,147);
  sprite.drawString("FPS"+String(fps), 295,147);
  sprite.drawString("By Lynnette", 280,160);
  //sprite.drawString(mqtt_msg, 280, 90);

  sprite.pushSprite(0, 0);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_POWER_ON, OUTPUT);
  digitalWrite(PIN_POWER_ON, HIGH);
  tft.init();
  tft.setRotation(1);
  sprite.createSprite(320, 170);
  sprite.setSwapBytes(true);
  sprite.setTextDatum(4);
  sprite.setTextColor(TFT_WHITE);
  sprite.setFreeFont(&Orbitron_Light_24);
  sprite.fillSprite(backColor);
  sprite.drawString("Lynnette Monitor", 160, 40);
  sprite.drawString("BOOTING...", 160, 70);
  sprite.drawString("Connecting WIFI...", 160, 100);
  sprite.pushSprite(0, 0);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  bupt_login(buptacc,buptpass);
  delay(500);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer,ntpServer2,ntpServer3);
  delay(50);
  IP=WiFi.localIP().toString();
  xTaskCreate(
    get_weather,
    "GetTimeString",
    10000,
    NULL,
    1,
  NULL);
  init_mqtt();
}

void loop() {
  float volt;
  unsigned int lastframe_time = millis();
  unsigned int volttime = millis();
  volt = (analogRead(4) * 2 * 3.3 * 1000) / 4096 / 1000.0;
  while(1){
    if(!client.connected()){
      client.connect("ESP32Client", mqttUser, mqttPassword);
      client.subscribe(mqttTopic);
    }
    client.loop();
    if(volt == (analogRead(4) * 2 * 3.3 * 1000) / 4096 / 1000.0){
      volttime = millis();
    }
    if(millis()-volttime>2000){
      volt = (analogRead(4) * 2 * 3.3 * 1000) / 4096 / 1000.0;
      volttime = millis();
    }
    //Serial.println(volt);
    // Simulate changing data
    // Update arcs with the new percentages
    if(t+1000<millis()){
      printLocalTime();
      t=millis();
    }
    unsigned int now_time = millis();
    int fps = 1000/((now_time - lastframe_time+1000)%1000);
    lastframe_time = now_time;
    updateArcs(volt,fps);
  }
}