#include "ViZGolfController.h"

#include <typeinfo>

#include <boost/algorithm/string.hpp>
#include <boost/chrono.hpp>
#include <boost/lexical_cast.hpp>

#include <stdio.h>  /* defines FILENAME_MAX */
#include <unistd.h>

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

        /* Message queues */
        this->MQController = nullptr;
        this->MQGolf = nullptr;

        /* Shared memory */
        this->SM = nullptr;

        /* Threads */
        this->golfThread = nullptr;
    }

    GolfController::~GolfController() {
        std::cout << "GolfController::~GolfController()" << std::endl;
    }

    bool GolfController::init() {
        std::cout << "GolfController::init()" << std::endl;
        
        if (!this->golfRunning) {
            try {
                //this->generateInstanceId();

                // Golf thread
                //this->golfThread = new b::thread(b::bind(&GolfController::launchGolf, this));
                this->golfRunning = true;
                //std::cout << "GolfController::init()" << std::endl;
            } catch (...) {
                throw;
            }
        }
        
        return true;
    }

    bool GolfController::isGolfRunning() { return this->golfRunning; }

    void GolfController::launchGolf() {
        //std::cout << "GolfController::launchGolf()" << std::endl;
        //char cCurrentPath[FILENAME_MAX];
        this->golfRunning = true;

        //if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath))) {
        //     return ;
        //}

        //cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */
        //printf("The current working directory is %s", cCurrentPath);

        this->golfArgs.push_back("/home/kimbring2/Twini-Golf/src/vizgolf/vizgolf");

        bpr::child golfProcess = bpr::execute(bpri::set_args(this->golfArgs), bpri::inherit_env());
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
    }

    void GolfController::generateSharedMemory() {
        std::string memory_name = "test";
        this->SM = new SharedMemory(memory_name);
    }

    void GolfController::generateMessageQueue() {
        //std::cout << "GolfController::generateMessageQueue()" << std::endl;

        std::string message_queue_name_1 = "test1";
        std::string message_queue_name_2 = "test2";
        this->MQGolf = new MessageQueue(message_queue_name_1);
        this->MQController = new MessageQueue(message_queue_name_2);
    }

    void GolfController::getValueFromSharedMemory() {
        printf("this->SM->getGameState()->BALL_STROKE_0: %d\n", this->SM->getGameState()->BALL_STROKE_0);
        //printf("this->SM->getGameState()->BALL_STROKE_1: %d\n", this->SM->getGameState()->BALL_STROKE_1);
        printf("this->SM->getGameState()->LEVEL_0: %d\n", this->SM->getGameState()->LEVEL_0);
        //printf("this->SM->getGameState()->LEVEL_1: %d\n", this->SM->getGameState()->LEVEL_1);

        printf("this->SM->getGameState()->BALL_POS_0.x: %d\n", this->SM->getGameState()->BALL_POS_0.x);
        printf("this->SM->getGameState()->BALL_POS_0.y: %d\n", this->SM->getGameState()->BALL_POS_0.y);
        //printf("this->SM->getGameState()->BALL_POS_1.x: %d\n", this->SM->getGameState()->BALL_POS_1.x);
        //printf("this->SM->getGameState()->BALL_POS_1.y: %d\n", this->SM->getGameState()->BALL_POS_1.y);

        //printf("this->SM->getGameState()->MAP_KILLCOUNT: %d\n", this->SM->getGameState()->MAP_KILLCOUNT);
    }

    void GolfController::sendCommand(std::string command) {
        //printf("MSG_CODE_COMMAND: %d\n", MSG_CODE_COMMAND);
        //printf("command.c_str(): %s\n", command.c_str());
        this->MQGolf->send(MSG_CODE_COMMAND, command.c_str());
    }
}