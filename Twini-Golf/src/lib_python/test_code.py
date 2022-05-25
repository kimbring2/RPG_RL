import vizgolf as vzg
import time
import cv2
import numpy as np
import random

game = vzg.GolfGame()

game.add_available_button(vzg.Button.MOVE_FORWARD_BACKWARD_DELTA)
game.add_available_button(vzg.Button.MOVE_LEFT_RIGHT_DELTA)
game.add_available_button(vzg.Button.TURN_LEFT_RIGHT_DELTA)
game.add_available_button(vzg.Button.LOOK_UP_DOWN_DELTA)
#game.button_update()

game.init()

print("init")
game.send_command("init")
#time.sleep(0.5)

#game.load_config()

available_buttons = game.get_available_buttons()
print("available_buttons: ", available_buttons)

#game.make_action([1,2,3])

SLOT_COUNT = vzg.SLOT_COUNT
print("SLOT_COUNT: ", SLOT_COUNT)

server_state = game.get_server_state()
#print("server_state.tic: ", server_state.tic)
#print("server_state.player_count: ", server_state.player_count)
#print("server_state.players_in_game: ", server_state.players_in_game)
#print("server_state.players_names: ", server_state.players_names)

game.update_state()

for i in range(0, 50000):
	game.make_action([1.0,2.0,3.0])

	game.update_state()
	#game.button_update()
	#game.sendCommand(str(i))
	#game.sendCommand("init")
	#print("")
	#time.sleep(0.5)
	#game.get_value_from_shared_memory()
	#time.sleep(0.5)
	#game.make_action([1,2,3])

	#screen_buffer.shape:  (4, 480, 640)
	state = game.get_state()
	#print("state.labels[0].object_name: ", state.labels[0].object_name)
	#print("state.screen_buffer.shape: ", state.screen_buffer.shape)
	#screen_buffer = np.transpose(screen_buffer, (1, 2, 0))
	#print("screen_buffer.shape: ", screen_buffer.shape)
	cv2.imshow("screen_buffer", state.screen_buffer)
	cv2.waitKey(1)

	x_action_float = random.uniform(-0.5, 0.5)
	y_action_float = random.uniform(-0.5, 0.5)

	x_action_string = "{:+1.4f}".format(x_action_float)
	#print("x_action_string: ", x_action_string)

	y_action_string = "{:+1.4f}".format(y_action_float)
	#print("y_action_string: ", y_action_string)

	action_string = "1$" + x_action_string + "$" + y_action_string + "$"

	#game.send_command(action_string)
	#time.sleep(0.1)

	#print("")

test.send_command("end")
#time.sleep(0.5)
