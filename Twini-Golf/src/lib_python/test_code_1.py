import golf
import time

test = golf.GolfGame()

#test.generateSharedMemory()

while True:
	#test.getValueFromSharedMemory()
	test.sendCommand("test")
	print("")
	time.sleep(0.5)