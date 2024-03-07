# MQTT-SERVER-MONITOR
ESP32S3 TFT screen (or Lilygo/TTgo tdisplay-s3) server/computer status monitor. ESP32S3 tft屏幕的服务器 电脑状态监视仪。  
Weather API/天气API接口申请地址：https://www.seniverse.com/  
申请免费版即可 
 ![abdf48028a1f4c382ee45979797acbf](https://github.com/Lynnette177/MQTT-SERVER-MONITOR/assets/68948483/d1cf4526-312b-4c86-a39c-9651ddc24e39)
# Usage
## Client
Using a esp32s3 with tft screen (better using lilygo T-displayS3).  
Define your WIFI SSID/PASSWORD in bupt.h. Also your BUPT Account/Password if you need auto login BUPT-portal in school.  
Define your Weather API key in weather.h.  
Define your mqtt server/port/account/password/topic in mqtt.h.  
Flash and enjoy. 

## Server
A simple python script. You can include this file in your other files, then start a thread with a while True loop,upload status every 2 seconds using time.sleep(2)  
Don't forget to set mqtt server/account/password/port!  
Then everything should work!  

## Features 
### Display(String):
Real-time weather/temperature/updated time  
Esp32's IP Address  
CPU Usage/temperature  
Memory Usage  
Memory Cached  
Total Memory  
Available Memory  
Swap Usage 
Date/Weekdays/Time  
Server Up Time  
ESP32's battery volt  
FPS  
### Arcs(Gauge): 
CPU Temperature, CPU Frequency,  
Mem used/Cached (in one arc different color)  
Swap Available   

# 中文
ESP32S3 tft屏幕的服务器 电脑状态监视仪。推荐使用lilygo T-displayS3。淘宝可以搜到。 

# 使用方法
## 客户端
使用一块有tft屏幕的esp32 推荐使用lilygo T-displayS3。淘宝可以搜到。  
在bupt.h中定义wifi名称和密码，如果需要自动登录北邮校园网，也要写好你的学号密码。  
在weather.h中定义心知天气api密钥。在上面的网址申请，免费。  
在mqtt.h中定义mqtt服务器、端口、账号、密码、订阅主题。  
刷进esp32就可以了。 
 
## 服务器
一个很简单的python脚本，使用psutil进行信息获取。你可以把这个文件直接include，然后启动一个无限循环的线程，每隔两秒上传一次即可  
不要忘记设置好mqtt服务器地址、账号、密码、端口号。  
 
## 功能 
### 文字显示:
实时天气、温度、天气更新时间  
ESP32的IP地址  
CPU用量、温度  
内存用量  
Cached的内存  
总内存  
可用内存  
Swap用量  
日期/星期/时间  
服务器运行时间  
ESP32电池电压  
渲染帧率  
### 仪表盘: 
CPU温度、频率  
内存已使用、Cached（在内存总量中不同颜色） 
可用swap分区量。 
