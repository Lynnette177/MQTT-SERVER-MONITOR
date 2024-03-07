import time
from paho.mqtt import publish
import json
import os
import psutil
import socket
#源码中只需要知道   ip + 端口 + 订阅的主题
HOST ="192.168.1.2"
PORT =1883
#client.on_publish = on_publish
alertCPUTempMax = 70
alertDiskUsagePercentMax = 95
alertLoadAvgMax = 9

def send_msg_mqtt():
    cpu_temperature = -128
    host_name = "localhost"
    try:
        cpu_temperature = (int(os.popen('cat /sys/class/thermal/thermal_zone0/temp').read())) / 1000
    except:
        pass
    alert = 0
    if cpu_temperature > alertCPUTempMax:
        alert = 1
    if psutil.getloadavg()[2] > alertLoadAvgMax:
        alert = 1
    data = {
        "cpu_percent": psutil.cpu_percent(),
        "cpu_freq": {
            "current": psutil.cpu_freq().current,
            "min": psutil.cpu_freq().min,
            "max": psutil.cpu_freq().max
        },
        "virtual_memory": {
            "total": psutil.virtual_memory().total/1024/1024,
            "available": psutil.virtual_memory().available/1024/1024,
            "percent": psutil.virtual_memory().percent,
            "used": psutil.virtual_memory().used/1024/1024,
            "free": psutil.virtual_memory().free/1024/1024,
                #"wired": psutil.virtual_memory().wired/1024/1024
        },
        "swap_memory": {
            "total": psutil.swap_memory().total/1024/1024,
            "used": psutil.swap_memory().used/1024/1024,
            "free": psutil.swap_memory().free/1024/1024,
            "percent": psutil.swap_memory().percent,
        },
        "up_time": int(time.time() - psutil.boot_time()),
        "cpu_temperature": cpu_temperature,
        "alert": alert
    }
  publish.single("NAS-STAT",json.dumps(data), qos = 0, hostname = HOST, port = PORT, client_id = "NAS",auth = {'username': "acc", 'password': "pass"})
