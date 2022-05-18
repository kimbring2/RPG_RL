#ifndef __VIZGOLF_GAME_PYTHON_H__
#define __VIZGOLF_GAME_PYTHON_H__

#include "ViZGolfGame.h"

#include <iostream>
#include <vector>

namespace vizgolf {
    class GolfGamePython : public GolfGame {

    public:
        GolfGamePython();

        // These functions are wrapped for manual GIL management
        void init();
        void launchGolf();
        void generateSharedMemory();
        void getValueFromSharedMemory();
        void generateMessageQueue();
        void sendCommand(std::string command);
    };
}

#endif
