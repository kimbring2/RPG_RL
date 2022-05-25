#ifndef __VIZGOLF_GAME_PYTHON_H__
#define __VIZGOLF_GAME_PYTHON_H__

#include "ViZGolfGame.h"

#include <iostream>
#include <vector>
#include "pybind11/include/pybind11/pybind11.h"
#include "pybind11/include/pybind11/stl.h" 
#include "pybind11/include/pybind11/numpy.h" 


namespace vizgolf {
    namespace pyb = pybind11;

    struct ServerStatePython {
        unsigned int tic;
        unsigned int playerCount;
        pyb::list playersInGame;
        pyb::list playersFrags;
        pyb::list playersNames;
        pyb::list playersAfk;
        pyb::list playersLastActionTic;
        pyb::list playersLastKillTic;
    };

    struct GameStatePython {
        unsigned int number;
        unsigned int tic;
        double reward;

        pyb::object gameVariables;

        pyb::object screenBuffer;
        pyb::object depthBuffer;
        pyb::object labelsBuffer;
        pyb::object automapBuffer;
        pyb::object audioBuffer;

        pyb::list labels;
        pyb::list objects;
        pyb::list sectors;
    };

    class GolfGamePython : public GolfGame {

    public:
        GolfGamePython();

        // These functions are wrapped for manual GIL management
        void init();
        double makeAction(pyb::object const &pyAction);
        //double makeAction_list(pyb::object const &_list){ return this->makeAction(_list); }
        //double makeAction_list_int(pyb::object const &_list, unsigned int _int){ return this->makeAction(_list, _int); }
        void updateSharedMemory();
        GameStatePython* getState();
        void sendCommand(std::string command);

        void addAvailableButton_btn(Button _btn) { this->addAvailableButton(_btn); }
        void setAvailableButtons(pyb::list const &pyButtons);
        ServerStatePython* getServerState();
        pyb::list getAvailableButtons();

    private:
        GameStatePython* pyState;
        std::vector<pyb::ssize_t> colorShape;
        
        template<class T> static std::vector<T> pyListToVector(pyb::list const &pyList);
        template<class T> static std::vector<T> pyArrayToVector(pyb::array_t<T> const &pyArray);
        template<class T> static std::vector<T> pyObjectToVector(pyb::object const &pyObject);
        template<class T> static pyb::list vectorToPyList(const std::vector<T>& vector);
        template<class T> static pyb::array_t<T> dataToNumpyArray(std::vector<pyb::ssize_t> dims, T *data);
    };
}

#endif
