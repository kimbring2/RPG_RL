#ifndef __VIZGOLF_CONTROLLER_H__
#define __VIZGOLF_CONTROLLER_H__

#include "ViZGolfSharedMemory.h"
#include "ViZGolfMessageQueue.h"

#include <string>
#include <vector>

#include "boost/process.hpp"
#include <boost/asio.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>

namespace vizgolf {
    namespace b         = boost;
    namespace bpl       = boost::placeholders;
    namespace ba        = boost::asio;
    namespace br        = boost::random;
    namespace bs        = boost::system;
    namespace bpr       = boost::process;
    namespace bpri      = boost::process::initializers;

#define INSTANCE_ID_LENGTH 10

/* Shared memory's settings */
#define SM_NAME_BASE        "ViZGolfSM"

/* Message queues' settings */
#define MQ_CTR_NAME_BASE    "ViZGolfMQCtr"
#define MQ_DOOM_NAME_BASE   "ViZGolfMQDoom"

/* Messages' codes */
#define MSG_CODE_DOOM_DONE              11
#define MSG_CODE_DOOM_CLOSE             12
#define MSG_CODE_DOOM_ERROR             13
#define MSG_CODE_DOOM_PROCESS_EXIT      14

#define MSG_CODE_TIC                    21
#define MSG_CODE_UPDATE                 22
#define MSG_CODE_TIC_AND_UPDATE         23
#define MSG_CODE_COMMAND                24
#define MSG_CODE_CLOSE                  25

#define MSG_CODE_SIG                    30
#define MSG_CODE_SIGINT                 30 + SIGINT
#define MSG_CODE_SIGABRT                30 + SIGABRT
#define MSG_CODE_SIGTERM                30 + SIGTERM

    class GolfController {

    public:
        GolfController();
        ~GolfController();

        bool init();
        bool isGolfRunning();
        void launchGolf();
        void generateSharedMemory();
        void generateMessageQueue();
        void getValueFromSharedMemory();
        void sendCommand(std::string command);

    private:
        /* Flow */
        /*------------------------------------------------------------------------------------------------------------*/
        bool golfRunning;
        bool golfWorking;
        std::vector<std::string> golfArgs;
        std::string instanceId;

        /* Message queues */
        /*------------------------------------------------------------------------------------------------------------*/
        MessageQueue *MQGolf;
        MessageQueue *MQController;

        /* Shared memory */
        /*------------------------------------------------------------------------------------------------------------*/
        SharedMemory *SM;

        void generateInstanceId();

        b::thread *golfThread;
    };
}

#endif
