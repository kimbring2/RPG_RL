#include "ViZGolfGame.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp> // for reading the shared object/dll path
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim_all.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <fstream>
#include <iostream>

#include <cstddef>
#include <cstring>

using namespace cv;

namespace vizgolf {
    GolfGame::GolfGame() {
        this->running = false;
        this->golfController = new GolfController();
        this->lastReward = 0;
        this->summaryReward = 0;
    }

    GolfGame::~GolfGame() {
        delete this->golfController;
        this->running = false;
    }

    bool GolfGame::init() {
        std::cout << "GolfGame::init()" << std::endl;

        if (!this->isRunning()) {
            std::string cfgOverrideFile = "./_golf.cfg";

            this->lastAction.resize(this->availableButtons.size());
            this->nextAction.resize(this->availableButtons.size());

            try {
                this->running = this->golfController->init();

                printf("this->golfController->disableAllButtons()\n");
                this->golfController->disableAllButtons();
                printf("this->availableButtons.size(): %d\n", this->availableButtons.size());
                for (unsigned int i = 0; i < this->availableButtons.size(); ++i) {
                    //printf("i: %d\n", i);
                    this->golfController->setButtonAvailable(this->availableButtons[i], true);
                }
            } catch (...) { 
                throw; 
            }

            return running;
        } else {
            return false;
        }
        
        return false;
    }

    bool GolfGame::loadConfig() {
        addAvailableButton(MOVE_LEFT);
        addAvailableButton(MOVE_RIGHT);
        addAvailableButton(ATTACK);

        std::cout << "GolfGame::loadConfig()" << std::endl;
        return true;
    }

    double GolfGame::makeAction(std::vector<double> const &actions) {
        this->setAction(actions);
        //this->advanceAction(tics);
        // this->getLastReward();
        double last_reward = 0.0;

        return last_reward;
    }

    void GolfGame::setAction(std::vector<double> const &actions) {
        if (!this->isRunning()) {
            std::cout << "ViZGolfIsNotRunningException" << std::endl;
            throw;
        }

        //for (unsigned int i = 0; i < actions.size(); ++i)
        //    printf("actions[i]: %f\n", actions[i]);

        //printf("this->availableButtons.size(): %d\n", this->availableButtons.size());
        //printf("this->nextAction.size(): %d\n", this->nextAction.size());
        for (unsigned int i = 0; i < this->availableButtons.size(); ++i) {
            //printf("this->availableButtons[i]: %d\n", this->availableButtons[i]);

            if (i < actions.size()) {
                this->nextAction[i] = actions[i];
            } else {
                this->nextAction[i] = 0;
            }

            this->golfController->setButtonState(this->availableButtons[i], this->nextAction[i]);
        }
    }

    bool GolfGame::buttonUpdate() {
        //printf("GolfGame::buttonUpdate()\n");

        this->golfController->disableAllButtons();
        for (unsigned int i = 0; i < this->availableButtons.size(); ++i) {
            //printf("i: %d\n", i);
            this->golfController->setButtonAvailable(this->availableButtons[i], true);
        }

        return true;
    }

    bool GolfGame::isRunning() {
        return this->running && this->golfController->isGolfRunning();
    }

    void GolfGame::updateState() {
        unsigned char *buf = this->golfController->getScreenBuffer();

        this->state = std::make_shared<GameState>();
        const size_t graySize = 640 * 480;
        const size_t colorSize = graySize * 4;

        this->state->screenBuffer = std::make_shared<std::vector<uint8_t>>(buf, buf + colorSize);
        SMGameState *smState = this->golfController->getGameState();

        unsigned int stroke_0 = this->golfController->getGameVariable(STROKE_0);
        unsigned int stroke_1 = this->golfController->getGameVariable(STROKE_1);

        unsigned int level_0 = this->golfController->getGameVariable(LEVEL_0);
        unsigned int level_1 = this->golfController->getGameVariable(LEVEL_1);


        //double doomFixedToDouble(int doomFixed) {
        //    return static_cast<double>(doomFixed) / 65536.0;
        //}   

        double reward = static_cast<double>(this->golfController->getReward());
        //printf("reward: %f\n", reward);

        this->state->reward = reward;

        this->lastReward = reward;
        this->summaryReward += reward;
        this->lastReward = reward;

        //SMInputState *SharedMemory::getInputState() {
        //    return static_cast<SMInputState *>(this->region[1].address);
        //}

        SMInputState *inputState = this->golfController->getInput();

        //printf("inputState->BT[0]: %f\n", inputState->BT[0]);
        //printf("inputState->BT[1]: %f\n", inputState->BT[1]);
        //printf("inputState->BT[2]: %f\n", inputState->BT[2]);

        //printf("BALL_STROKE_0: %d\n", stroke_0);
        //printf("BALL_STROKE_1: %d\n", stroke_1);
        //printf("LEVEL_0: %d\n", level_0);
        //printf("LEVEL_1: %d\n", level_1);

        //for (unsigned int i = 0; i < smState->LABEL_COUNT; ++i) {
            //printf("smState->LABEL[i].objectName: %s\n", smState->LABEL[i].objectName);
            //printf("smState->LABEL[i].position[0]: %d\n", smState->LABEL[i].position[0]);
            //printf("smState->LABEL[i].position[1]: %d\n", smState->LABEL[i].position[1]);
            //printf("smState->LABEL[i].objectId: %d\n", smState->LABEL[i].objectId);
        //}

        //this->state->labels.clear();

        /* Update labels */
        size_t labelPartSize = offsetof(struct Label, objectName) - offsetof(struct Label, value);
        for (unsigned int i = 0; i < smState->LABEL_COUNT; ++i) {
            this->state->labels.emplace_back();
            std::memcpy(&this->state->labels.back().value, &smState->LABEL[i].value, labelPartSize);
            //printf("smState->LABEL[i].objectName: %s\n", smState->LABEL[i].objectName);
            this->state->labels.back().objectName = std::string(smState->LABEL[i].objectName);
        }
    }

    bool GolfGame::sendCommand(std::string command) {
        this->golfController->sendCommand(command);
        return true;
    }

    void GolfGame::addAvailableButton(Button button) {
        if (!this->isRunning() && std::find(this->availableButtons.begin(),
                                            this->availableButtons.end(), button) == this->availableButtons.end()) {
            //std::cout << "GolfGame::addAvailableButton()" << std::endl;
            //std::cout << "button: " << button << std::endl;
            this->availableButtons.push_back(button);
        }
    }

    void GolfGame::setAvailableButtons(std::vector<Button> buttons) {
        this->clearAvailableButtons();
        for(auto i : buttons) this->addAvailableButton(i);
    }

    std::vector<Button> GolfGame::getAvailableButtons() {
        return this->availableButtons;
    }

    void GolfGame::clearAvailableButtons() {
        if (!this->isRunning()) this->availableButtons.clear();
    }
}