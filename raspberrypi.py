import serial
import time
import requests
port = serial.Serial("/dev/ttyUSB0", "9600")
dbHost = "leehgyu.iptime.org"   #Data base url
dbPort = "8080"                 #Data base port
strParkID = "1"
strCarExist = "1"
flag = 3
chargestatus = 0.0
find_link = "http://"+dbHost+":"+dbPort+"/read.php"

port.reset_input_buffer()
while True:
	
	x = requests.get(find_link)
	datatext = x.text
	rows = datatext.split(" ")[0].split("/")
	relay_rows = [int(rows[0].split(",")[4]),int(rows[1].split(",")[4]),int(rows[2].split(",")[4])]
	if port.in_waiting >0 :
		line = port.readline().decode().rstrip()
	for i in [0,1,2]:
		if(relay_rows[i] == 1):
			#port.reset_input_buffer()
			command = 'a'
			port.write(command.encode())
			#print("relay on")
			if port.in_waiting >0 :
				line = port.readline().decode().rstrip()
				chargestatus+= float(line)
				print("cs:", chargestatus)
				strchargestatus = str(int(chargestatus))
				strparkID = str(i+1)
				insert_link = "http://"+dbHost+":"+dbPort+"/charge_status.php?ParkID="+strparkID+"&ChargeStatus="+strchargestatus
				x = requests.get(insert_link)
				flag = i
		if(flag == i and relay_rows[i] == 0):
			port.reset_input_buffer()
			command = 'b'
			port.write(command.encode())
			chargestatus = 0.0
			strchargestatus = str(int(chargestatus))
			strparkID = str(i+1)
			insert_link = "http://"+dbHost+":"+dbPort+"/charge_status.php?ParkID="+strparkID+"&ChargeStatus="+strchargestatus
			x = requests.get(insert_link)
			#print("relay low")
port.close()
