#ifndef __VIZGOLF_GAME_H__
#define __VIZGOLF_GAME_H__

#include "ViZGolfTypes.h"
#include "ViZGolfController.h"

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

namespace vizgolf {
    class GolfController;

    class GolfGame {

    public:
        GolfGame();
        virtual ~GolfGame();

        /* Flow Control */
        /*------------------------------------------------------------------------------------------------------------*/
        bool init();
        bool loadConfig();
        bool isRunning();
        void updateState();
        bool sendCommand(std::string command);

        double makeAction(std::vector<double> const &actions);
        std::vector<Button> getAvailableButtons();
        void setAction(std::vector<double> const &actions);
        void setAvailableButtons(std::vector<Button> buttons);
        void addAvailableButton(Button button);
        void clearAvailableButtons();
        bool buttonUpdate();

    protected:
        GolfController *golfController;
        bool running;

        GameStatePtr state;

        std::vector<Button> availableButtons;
        std::vector<double> lastAction;
        std::vector<double> nextAction;

        /* Rewards */
        /*------------------------------------------------------------------------------------------------------------*/
        double lastReward;
        double summaryReward;
    };
}

#endif