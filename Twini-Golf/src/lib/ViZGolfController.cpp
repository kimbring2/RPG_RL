#include "ViZGolfController.h"

#include <typeinfo>

#include <boost/algorithm/string.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>

#include <stdio.h>
#include <unistd.h>

using namespace cv;

#define GetCurrentDir getcwd

namespace vizgolf {
    namespace bal       = boost::algorithm;
    namespace bc        = boost::chrono;
    namespace bfs       = boost::filesystem;

    /* Public methods */
    /*----------------------------------------------------------------------------------------------------------------*/
    GolfController::GolfController() {
        /* Flow control */
        this->golfRunning = false;
        this->golfWorking = false;

        /* Threads */
        this->signalThread = nullptr;
        this->golfThread = nullptr;

        /* Message queues */
        this->MQController = nullptr;
        this->MQGolf = nullptr;

        /* Shared memory */
        this->SM = nullptr;
        this->gameState = nullptr;
        this->input = nullptr;
        this->screenBuffer = nullptr;

        /* Threads */
        this->golfThread = nullptr;

        this->_input = new SMInputState();
    }

    GolfController::~GolfController() {
        //std::cout << "GolfController::~GolfController()" << std::endl;
        ;
    }

    bool GolfController::init() {
        //std::cout << "GolfController::init()" << std::endl;
        
        if (!this->golfRunning) {
            try {
                this->generateInstanceId();

                // Golf thread
                this->golfThread = new b::thread(b::bind(&GolfController::launchGolf, this));
                this->golfRunning = true;

                // Signal handle thread
                this->signalThread = new b::thread(b::bind(&GolfController::handleSignals, this));

                this->MQGolf = new MessageQueue(MQ_GOLF_NAME_BASE + this->instanceId);
                this->MQController = new MessageQueue(MQ_CTR_NAME_BASE + this->instanceId);

                this->waitForGolfStart();
                this->SM = new SharedMemory(SM_NAME_BASE + this->instanceId);
                this->gameState = this->SM->getGameState();
                this->input = this->SM->getInputState();
                this->screenBuffer = this->SM->getScreenBuffer();

                *this->input = *this->_input;     
            } catch (...) {
                throw;
            }
        }
        
        return true;
    }

    void GolfController::tick() {
        if (this->golfRunning) {
            this->MQGolf->send(MSG_CODE_TIC);
            //this->waitForGolfWork();
        }
    }

    void GolfController::waitForGolfStart() {
        this->golfWorking = true;
        this->golfRunning = this->receiveMQMsg();
        this->golfWorking = false;
    }

    bool GolfController::receiveMQMsg() {
        Message msg = this->MQController->receive();

        //printf("msg.code: %d\n", msg.code);
        //printf("std::string(msg.command): %s\n", std::string(msg.command));

        return true;
    }

    void GolfController::handleSignals() {
        //std::cout << "GolfController::handleSignals()" << std::endl;

        this->ioService = new ba::io_service();
        ba::signal_set signals(*this->ioService, SIGINT, SIGABRT, SIGTERM);
        signals.async_wait(b::bind(signalHandler, b::ref(signals), this, bpl::_1, bpl::_2));

        this->ioService->run();
    }

    void GolfController::signalHandler(ba::signal_set &signal, GolfController *controller, const bs::error_code &error,
                                       int sigNumber) {
        controller->intSignal(sigNumber);
    }

    void GolfController::intSignal(int sigNumber) {
        this->MQGolf->send(MSG_CODE_CLOSE);
        this->MQController->send(static_cast<uint8_t >(MSG_CODE_SIG + sigNumber));
    }

    bool GolfController::isGolfRunning() { return this->golfRunning; }

    void GolfController::launchGolf() {
        //std::cout << "GolfController::launchGolf()" << std::endl;

        char tmp[256];
        getcwd(tmp, 256);
        //std::cout << "Executable file location: " << std::string(tmp) + "/twingolf" << std::endl;

        this->golfRunning = true;

        //this->golfArgs.push_back("/home/kimbring2/Twini-Golf/src/vizgolf/twingolf");
        this->golfArgs.push_back(std::string(tmp) + "/twingolf");
        this->golfArgs.push_back("+viz_instance_id");
        this->golfArgs.push_back(this->instanceId);

        bpr::child golfProcess = bpr::execute(bpri::set_args(this->golfArgs), bpri::inherit_env());
        //bpr::wait_for_exit(golfProcess);
    }

    void GolfController::generateInstanceId() {
        std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
        this->instanceId = "";

        br::uniform_int_distribution<size_t> charDist(0, static_cast<size_t>(chars.length() - 1));
        br::mt19937 rng;
        rng.seed((unsigned int) bc::high_resolution_clock::now().time_since_epoch().count());

        for (int i = 0; i < INSTANCE_ID_LENGTH; ++i) {
            this->instanceId += chars[charDist(rng)];
        }

        //for(int i = 0 ; i < strlen(this->instanceId) ; i ++ ){
        //    std::cout << "this->instanceId[i]" << this->instanceId[i] << std::endl;
        //}
        //printf("(this->instanceId).c_str(): %s\n", (this->instanceId).c_str());
    }

    uint8_t *const GolfController::getScreenBuffer() {
        return this->screenBuffer;
    }

    SMGameState *const GolfController::getGameState() { 
        return this->gameState; 
    }

    SMInputState *const GolfController::getInput() { return this->input; }

    /* GameVariables getters */
    /*----------------------------------------------------------------------------------------------------------------*/
    double GolfController::getGameVariable(GameVariable var) {
        switch (var) {
            case STROKE_0 :
                return this->gameState->BALL_STROKE_0;
            case STROKE_1 :
                return this->gameState->BALL_STROKE_1;
            case LEVEL_0 :
                return this->gameState->LEVEL_0;
            case LEVEL_1:
                return this->gameState->LEVEL_1;
        }
    }

    double GolfController::getReward() { 
        double reward = this->gameState->REWARD; 
        //this->gameState->REWARD = 0.0;

        return reward; 
        //return 5.0;
    }

    void GolfController::sendCommand(std::string command) {
        //printf("MSG_CODE_COMMAND: %d\n", MSG_CODE_COMMAND);
        //printf("command.c_str(): %s\n", command.c_str());
        this->MQGolf->send(MSG_CODE_COMMAND, command.c_str());
        //printf("this->MQGolf->send(MSG_CODE_COMMAND, command.c_str())\n");

        Message received_msg = this->MQController->receive();
        //printf("received_msg.code: %d\n", received_msg.code);
        //printf("received_msg.command: %s\n", received_msg.command);
    }

    void GolfController::setButtonState(Button button, double state) {
        //printf("GolfController::setButtonState\n");
        //this->input->BT[0] = 1.0;
        //this->input->BT[1] = 3.0;
        //this->input->BT[2] = 5.0;
        //printf("this->_input->BT[0]: %f\n", this->input->BT[0]);
        //printf("this->_input->BT[1]: %f\n", this->input->BT[1]);
        //printf("this->input->BT[2]: %f\n", this->input->BT[2]);

        if (button < BUTTON_COUNT && button >= 0 && this->golfRunning) {
            //printf("button: %d\n", button);
            //printf("state: %f\n", state);
            this->input->BT[button] = state;
        }
    }

    void GolfController::disableAllButtons() {
        for (int i = 0; i < BUTTON_COUNT; ++i) {
            //printf("i: %d\n", i);
            if (this->golfRunning) {
                this->input->BT_AVAILABLE[i] = 0;
            }
        }
    }

    void GolfController::setButtonAvailable(Button button, bool allow) {
        //printf("GolfController::setButtonAvailable\n");
        //printf("button: %d\n", button);
        //printf("BUTTON_COUNT: %d\n", BUTTON_COUNT);
        //printf("this->golfRunning: %d\n", this->golfRunning);

        if (button < BUTTON_COUNT && button >= 0) {
            if (this->golfRunning) {
                //printf("button: %d\n", button);
                //printf("allow: %d\n", allow);
                this->input->BT_AVAILABLE[button] = allow;
            }
        }
    }
}