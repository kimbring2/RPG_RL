#ifndef __VIZGOLF_CONTROLLER_H__
#define __VIZGOLF_CONTROLLER_H__

#include "ViZGolfTypes.h"
#include "ViZGolfSharedMemory.h"
#include "ViZGolfMessageQueue.h"

#include <string>
#include <vector>

#include "boost/process.hpp"
#include <boost/asio.hpp>
#include <boost/random.hpp>
#include <boost/thread.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
#define MQ_GOLF_NAME_BASE   "ViZGolfMQGolf"

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
        void tick();
        uint8_t *const getScreenBuffer();
        SMInputState *const getInput();
        SMGameState *const getGameState();
        double getGameVariable(GameVariable var);
        void sendCommand(std::string command);
        void setButtonState(Button button, double state);
        void disableAllButtons();
        void setButtonAvailable(Button button, bool set);
        double getReward();

        void handleSignals();
        static void signalHandler(ba::signal_set &signal, GolfController *controller,
                                  const bs::error_code &error, int sigNumber);
        void intSignal(int sigNumber);
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
        SMInputState *input;
        SMInputState *_input;
        SMGameState *gameState;
        uint8_t *screenBuffer;

        bool receiveMQMsg();
        void waitForGolfStart();
        void generateInstanceId();

        b::thread *golfThread;

        /* Threads */
        /*------------------------------------------------------------------------------------------------------------*/
        ba::io_service *ioService;
        b::thread *signalThread;
    };
}

#endif
