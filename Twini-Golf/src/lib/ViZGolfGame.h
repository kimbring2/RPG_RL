#ifndef __VIZGOLF_GAME_H__
#define __VIZGOLF_GAME_H__

#include "ViZGolfController.h"

#include <list>
#include <memory>
#include <string>
#include <vector>

namespace vizgolf {
    class GolfController;

    class GolfGame {

    public:
        GolfGame();
        virtual ~GolfGame();

        /* Flow Control */
        /*------------------------------------------------------------------------------------------------------------*/
        bool init();
        bool isRunning();
        bool launchGolf();
        bool generateSharedMemory();
        bool getValueFromSharedMemory();
        bool generateMessageQueue();
        bool sendCommand(std::string command);

    protected:
        GolfController *golfController;
        bool running;
    };
}

#endif