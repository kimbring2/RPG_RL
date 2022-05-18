#include "ViZGolfGame.h"

#include <cstddef>
#include <cstring>

namespace vizgolf {
    GolfGame::GolfGame() {
        this->running = false;
        this->golfController = new GolfController();
    }

    GolfGame::~GolfGame() {
        delete this->golfController;
        this->running = false;
    }

    bool GolfGame::init() {
        //std::cout << "GolfGame::init()" << std::endl;

        if (!this->isRunning()) {
            std::string cfgOverrideFile = "./_golf.cfg";

            try {
                this->running = this->golfController->init();
            } catch (...) { 
                throw; 
            }

            return running;
        } else {
            return false;
        }
        
        return false;
    }

    bool GolfGame::isRunning() {
        return this->running && this->golfController->isGolfRunning();
    }

    bool GolfGame::launchGolf() {
        this->golfController->launchGolf();
        return true;
    }

    bool GolfGame::generateSharedMemory() {
        this->golfController->generateSharedMemory();
        return true;
    }

    bool GolfGame::getValueFromSharedMemory() {
        this->golfController->getValueFromSharedMemory();
        return true;
    }

    bool GolfGame::generateMessageQueue() {
        this->golfController->generateMessageQueue();
        return true;
    }

    bool GolfGame::sendCommand(std::string command) {
        this->golfController->sendCommand(command);
        return true;
    }
}