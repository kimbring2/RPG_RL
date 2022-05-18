#ifndef __GOLF_GAME_H__
#define __GOLF_GAME_H__

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
        void close();
        bool isRunning();

    protected:
        bool running;
    };
}

#endif