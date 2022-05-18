#include "ViZGolfGamePython.h"
#include "ViZGolfController.h"

#include <cstddef>
#include <cstring>

namespace vizgolf {
    GolfGamePython::GolfGamePython() {
        std::cout << "GolfGamePython::GolfGamePython()" << std::endl;
    }

    // These functions are wrapped for manual GIL management
    void GolfGamePython::init() {
        //std::cout << "GolfGamePython::init()" << std::endl;
        GolfGame::init();
    }

    void GolfGamePython::launchGolf() {
        //std::cout << "GolfGamePython::launchGolf()" << std::endl;
        GolfGame::launchGolf();
    }

    void GolfGamePython::generateSharedMemory() {
        //std::cout << "GolfGamePython::generateSharedMemory()" << std::endl;
        GolfGame::generateSharedMemory();
    }

    void GolfGamePython::getValueFromSharedMemory() {
        //std::cout << "GolfGamePython::getValueFromSharedMemory()" << std::endl;
        GolfGame::getValueFromSharedMemory();
    }

    void GolfGamePython::generateMessageQueue() {
        //std::cout << "GolfGamePython::generateMessageQueue()" << std::endl;
        GolfGame::generateMessageQueue();
    }

    void GolfGamePython::sendCommand(std::string command) {
        //std::cout << "GolfGamePython::sendCommand()" << std::endl;
        GolfGame::sendCommand(command);
    }
}