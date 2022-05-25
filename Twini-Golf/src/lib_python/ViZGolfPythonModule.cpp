//#include "../vizgolf/Ball.h"
//#include "../vizgolf/Entity.h"
//#include "../src/Hole.h"
//#include "../vizgolf/Math.h"
//#include "../vizgolf/RenderWindow.h"
//#include "../vizgolf/Tile.h"

#include "ViZGolfConsts.h"
#include "ViZGolfGamePython.h"

#include "pybind11/include/pybind11/pybind11.h"
#include "pybind11/include/pybind11/stl.h" 
#include "pybind11/include/pybind11/numpy.h" 

namespace pyb = pybind11;
using namespace std;
using namespace vizgolf;

PYBIND11_MODULE(vizgolf, vz)
{
    #define CONST_2_PYT(c) vz.attr(#c) = c
    #define ENUM_VAL_2_PYT(v).value(#v, v)
    #define ENUM_CLASS_VAL_2_PYT(c, v).value(#v, c::v)
    #define FUNC_2_PYT(f) vz.def(#f, f)
    #define CLASS_FUNC_2_PYT(c, f).def(#f, &c::f)

    CONST_2_PYT(SLOT_COUNT);
    CONST_2_PYT(MAX_PLAYERS);
    CONST_2_PYT(MAX_PLAYER_NAME_LENGTH);
    CONST_2_PYT(USER_VARIABLE_COUNT);
    CONST_2_PYT(DEFAULT_TICRATE);
    CONST_2_PYT(DEFAULT_FPS);
    CONST_2_PYT(DEFAULT_FRAMETIME_MS);
    CONST_2_PYT(DEFAULT_FRAMETIME_S);
    CONST_2_PYT(BINARY_BUTTON_COUNT);
    CONST_2_PYT(DELTA_BUTTON_COUNT);
    CONST_2_PYT(BUTTON_COUNT);

    pyb::enum_<Mode>(vz, "Mode")
        ENUM_VAL_2_PYT(PLAYER)
        ENUM_VAL_2_PYT(SPECTATOR)
        ENUM_VAL_2_PYT(ASYNC_PLAYER)
        ENUM_VAL_2_PYT(ASYNC_SPECTATOR);

    pyb::enum_<ScreenFormat>(vz, "ScreenFormat")
        ENUM_VAL_2_PYT(CRCGCB)
        ENUM_VAL_2_PYT(RGB24)
        ENUM_VAL_2_PYT(RGBA32)
        ENUM_VAL_2_PYT(ARGB32)
        ENUM_VAL_2_PYT(CBCGCR)
        ENUM_VAL_2_PYT(BGR24)
        ENUM_VAL_2_PYT(BGRA32)
        ENUM_VAL_2_PYT(ABGR32)
        ENUM_VAL_2_PYT(GRAY8)
        ENUM_VAL_2_PYT(DOOM_256_COLORS8);

    pyb::enum_<Button>(vz, "Button")
        ENUM_VAL_2_PYT(ATTACK)
        ENUM_VAL_2_PYT(USE)
        ENUM_VAL_2_PYT(JUMP)
        ENUM_VAL_2_PYT(CROUCH)
        ENUM_VAL_2_PYT(TURN180)
        ENUM_VAL_2_PYT(ALTATTACK)
        ENUM_VAL_2_PYT(RELOAD)
        ENUM_VAL_2_PYT(ZOOM)
        ENUM_VAL_2_PYT(SPEED)
        ENUM_VAL_2_PYT(STRAFE)
        ENUM_VAL_2_PYT(MOVE_RIGHT)
        ENUM_VAL_2_PYT(MOVE_LEFT)
        ENUM_VAL_2_PYT(MOVE_BACKWARD)
        ENUM_VAL_2_PYT(MOVE_FORWARD)
        ENUM_VAL_2_PYT(TURN_RIGHT)
        ENUM_VAL_2_PYT(TURN_LEFT)
        ENUM_VAL_2_PYT(LOOK_UP)
        ENUM_VAL_2_PYT(LOOK_DOWN)
        ENUM_VAL_2_PYT(MOVE_UP)
        ENUM_VAL_2_PYT(MOVE_DOWN)
        ENUM_VAL_2_PYT(LAND)
        ENUM_VAL_2_PYT(SELECT_WEAPON1)
        ENUM_VAL_2_PYT(SELECT_WEAPON2)
        ENUM_VAL_2_PYT(SELECT_WEAPON3)
        ENUM_VAL_2_PYT(SELECT_WEAPON4)
        ENUM_VAL_2_PYT(SELECT_WEAPON5)
        ENUM_VAL_2_PYT(SELECT_WEAPON6)
        ENUM_VAL_2_PYT(SELECT_WEAPON7)
        ENUM_VAL_2_PYT(SELECT_WEAPON8)
        ENUM_VAL_2_PYT(SELECT_WEAPON9)
        ENUM_VAL_2_PYT(SELECT_WEAPON0)
        ENUM_VAL_2_PYT(SELECT_NEXT_WEAPON)
        ENUM_VAL_2_PYT(SELECT_PREV_WEAPON)
        ENUM_VAL_2_PYT(DROP_SELECTED_WEAPON)
        ENUM_VAL_2_PYT(ACTIVATE_SELECTED_ITEM)
        ENUM_VAL_2_PYT(SELECT_NEXT_ITEM)
        ENUM_VAL_2_PYT(SELECT_PREV_ITEM)
        ENUM_VAL_2_PYT(DROP_SELECTED_ITEM)
        ENUM_VAL_2_PYT(LOOK_UP_DOWN_DELTA)
        ENUM_VAL_2_PYT(TURN_LEFT_RIGHT_DELTA)
        ENUM_VAL_2_PYT(MOVE_FORWARD_BACKWARD_DELTA)
        ENUM_VAL_2_PYT(MOVE_LEFT_RIGHT_DELTA)
        ENUM_VAL_2_PYT(MOVE_UP_DOWN_DELTA)
        .export_values();

    pyb::enum_<GameVariable>(vz, "GameVariable")
        ENUM_VAL_2_PYT(STROKE_0)
        ENUM_VAL_2_PYT(STROKE_1)
        ENUM_VAL_2_PYT(LEVEL_0)
        ENUM_VAL_2_PYT(LEVEL_1);

    #define LABEL_CLASS Label
    pyb::class_<LABEL_CLASS>(vz, "Label")
        .def_readonly("object_id", &LABEL_CLASS::objectId)
        .def_readonly("object_name", &LABEL_CLASS::objectName)
        .def_readonly("value", &LABEL_CLASS::value)
        .def_readonly("x", &LABEL_CLASS::x)
        .def_readonly("y", &LABEL_CLASS::y)
        .def_readonly("width", &LABEL_CLASS::width)
        .def_readonly("height", &LABEL_CLASS::height)
        .def_readonly("object_position_x", &LABEL_CLASS::objectPositionX)
        .def_readonly("object_position_y", &LABEL_CLASS::objectPositionY)
        .def_readonly("object_position_z", &LABEL_CLASS::objectPositionZ)
        .def_readonly("object_angle", &LABEL_CLASS::objectAngle)
        .def_readonly("object_pitch", &LABEL_CLASS::objectPitch)
        .def_readonly("object_roll", &LABEL_CLASS::objectRoll)
        .def_readonly("object_velocity_x", &LABEL_CLASS::objectVelocityX)
        .def_readonly("object_velocity_y", &LABEL_CLASS::objectVelocityY)
        .def_readonly("object_velocity_z", &LABEL_CLASS::objectVelocityZ);

    pyb::class_<GameStatePython>(vz, "GameState")
        .def_readonly("number", &GameStatePython::number)
        .def_readonly("tic", &GameStatePython::tic)
        .def_readonly("game_variables", &GameStatePython::gameVariables)
        .def_readonly("reward", &GameStatePython::reward)

        .def_readonly("screen_buffer", &GameStatePython::screenBuffer)
        .def_readonly("audio_buffer", &GameStatePython::audioBuffer)
        .def_readonly("depth_buffer", &GameStatePython::depthBuffer)
        .def_readonly("labels_buffer", &GameStatePython::labelsBuffer)
        .def_readonly("automap_buffer", &GameStatePython::automapBuffer)

        .def_readonly("labels", &GameStatePython::labels)
        .def_readonly("objects", &GameStatePython::objects)
        .def_readonly("sectors", &GameStatePython::sectors);

    pyb::class_<ServerStatePython>(vz, "ServerState")
        .def_readonly("tic", &ServerStatePython::tic)
        .def_readonly("player_count", &ServerStatePython::playerCount)
        .def_readonly("players_in_game", &ServerStatePython::playersInGame)
        .def_readonly("players_names", &ServerStatePython::playersNames)
        .def_readonly("players_frags", &ServerStatePython::playersFrags)
        .def_readonly("players_afk", &ServerStatePython::playersAfk)
        .def_readonly("players_last_action_tic", &ServerStatePython::playersLastActionTic)
        .def_readonly("players_last_kill_tic", &ServerStatePython::playersLastKillTic);

    pyb::class_<GolfGamePython>(vz, "GolfGame")
        .def(pyb::init<>())
        .def("init", &GolfGamePython::init)
        .def("load_config", &GolfGamePython::loadConfig)
        .def("update_state", &GolfGamePython::updateState)
        .def("set_available_buttons", &GolfGamePython::setAvailableButtons)
        .def("get_available_buttons", &GolfGamePython::getAvailableButtons)
        .def("get_state", &GolfGamePython::getState)
        .def("get_server_state", &GolfGamePython::getServerState)
        .def("make_action", &GolfGamePython::makeAction)
        .def("add_available_button", &GolfGamePython::addAvailableButton_btn)
        .def("button_update", &GolfGamePython::buttonUpdate)
        .def("send_command", &GolfGamePython::sendCommand);
}

