import golf
import time

test = golf.GolfGame()

test.generateMessageQueue()
test.generateSharedMemory()

time.sleep(1.0)

test.launchGolf()

time.sleep(1.0)

print("init")
test.sendCommand("init")
time.sleep(0.5)

#test.getValueFromSharedMemory()
#time.sleep(0.5)

#test.sendCommand("1$+0.0000$-0.1000$")
#time.sleep(0.1)

#time.sleep(0.5)

#print("")

#test.sendCommand("end")
#time.sleep(2)
#test.getValueFromSharedMemory()

for i in range(0, 5):
	#test.getValueFromSharedMemory()
	#test.sendCommand(str(i))
	#test.sendCommand("init")
	#print("")
	#time.sleep(0.5)
	test.generateSharedMemory()
	test.getValueFromSharedMemory()
	#time.sleep(0.5)

	test.sendCommand("1$+0.0000$-0.1000$")
	time.sleep(0.5)

	#print("")

test.sendCommand("end")
	#time.sleep(0.5)