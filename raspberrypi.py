# myobj = {"ParkID" : strParkID, "CarExist" : strCarExist}
# r=requests.post(link, json=myobj)
# print(r.text)

# x = requests.get("http://"+dbHost+":"+dbPort+"/read.php")
# print(x.text)

import serial
import time
import requests
port = serial.Serial("/dev/ttyACM0", "9600")
dbHost = "leehgyu.iptime.org"   #Data base url
dbPort = "8080"                 #Data base port
strParkID = "1"
strCarExist = "1"
find_link = "http://"+dbHost+":"+dbPort+"/read.php"

while True:
    try:
        x = requests.get(find_link)
        datatext = x.text
        rows = datatext.split(" ")[0].split("/")
        print(rows)
        relay_rows = [rows[0].split(",")[4],rows[1].split(",")[4],rows[2].split(",")[4]]
        print(relay_rows)
        for i in [0,1,2]:
            if(relay_rows[i]=='1'):
                command = "start" #relay = 1 이면 보냄
                port.write(command.encode())
                time.sleep(1)
            #print(relay_rows[i])
                flag = True
                st_time = time.time()
                while(flag):
                    if(port.readable()):
                        data_arduino = port.readline()	# 시리얼 포트에서 읽어들인 값
                        print(data_arduino)
                        strParkID = str(i)
                        insert_link = "http://"+dbHost+":"+dbPort+"/insert.php?ParkID="+strParkID+"&CarExist="+strCarExist
                        x = requests.get(insert_link)
                        if(time.time()- st_time >= 30):
                            flag = False
                            finish_link = "http://"+dbHost+":"+dbPort+"/insert.php?ParkID="+strParkID+"&CarExist="+strCarExist
                            x = requests.get(insert_link)
    except KeyboardInterrupt:
        break
                
port.close()
