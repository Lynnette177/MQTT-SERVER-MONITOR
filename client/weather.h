#include"bupt.h"
#include <ArduinoJson.h>
String API = "";
String WeatherURL = "";
String CITY = "北京";
String url_xinzhi = "";
String Weather = "0";
String temperature = "0";
String lastupdatetimepart = "";
String GitURL()
{
  url_xinzhi =  "https://api.seniverse.com/v3/weather/now.json?key=";
  url_xinzhi += API;
  url_xinzhi += "&location=";
  url_xinzhi += CITY;
  url_xinzhi += "&language=en&unit=c";
  return url_xinzhi;
}
void ParseWeather()
{  
  HTTPClient httpweather;
  String url = GitURL();
  DynamicJsonDocument doc(1024); //分配内存,动态
  httpweather.begin(url);
  int httpGet = httpweather.GET();
  if(httpGet > 0){
    Serial.printf("HTTPGET is %d",httpGet);
    if(httpGet == HTTP_CODE_OK){
      String json = httpweather.getString();
      Serial.println(json);
      deserializeJson(doc, json);
      Weather = doc["results"][0]["now"]["text"].as<String>();
      temperature = doc["results"][0]["now"]["temperature"].as<String>();
      const char* lastUpdate = doc["results"][0]["last_update"];
      // 找到T和+的位置
      int tIndex = String(lastUpdate).indexOf('T');
      int plusIndex = String(lastUpdate).indexOf('+');
      lastupdatetimepart = String(lastUpdate).substring(tIndex + 1, plusIndex);

      Serial.println(Weather);
      Serial.println(temperature);
      Serial.println(lastupdatetimepart);

    }
    else{Serial.printf("ERROR1!!");}
  }
  else{Serial.printf("ERROR2!!");}
  httpweather.end();
}