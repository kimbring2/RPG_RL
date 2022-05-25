#include "ViZGolfGamePython.h"
#include "ViZGolfController.h"

#include <cstddef>
#include <cstring>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

namespace vizgolf {
    GolfGamePython::GolfGamePython() {
        std::cout << "GolfGamePython::GolfGamePython()" << std::endl;
    }

    // These functions are wrapped for manual GIL management
    void GolfGamePython::init() {
        std::cout << "GolfGamePython::init()" << std::endl;
        GolfGame::init();

        this->colorShape.resize(3);
        this->colorShape[0] = 480;
        this->colorShape[1] = 640;
        this->colorShape[2] = 4;
    }

    double GolfGamePython::makeAction(pyb::object const &pyAction) {
        auto action = GolfGamePython::pyObjectToVector<double>(pyAction);
        double last_reward = 0.0;

        return GolfGame::makeAction(action);
    }

    GameStatePython* GolfGamePython::getState() {
        this->pyState = new GameStatePython();
        this->pyState->screenBuffer = GolfGamePython::dataToNumpyArray(this->colorShape, this->state->screenBuffer->data());
        
        //this->state->labels != nullptr
        this->pyState->labels = GolfGamePython::vectorToPyList<Label>(this->state->labels);
        this->pyState->reward = this->state->reward;
        //printf("this->state->labels[0].objectName: %s\n", this->state->labels[0].objectName);
        //printf("this->state->labels[0].objectId: %d\n", this->state->labels[0].objectId);

        return this->pyState;
    }

    void GolfGamePython::sendCommand(std::string command) {
        //std::cout << "GolfGamePython::sendCommand()" << std::endl;
        GolfGame::sendCommand(command);
    }

    void GolfGamePython::setAvailableButtons(pyb::list const &pyButtons){
        GolfGame::setAvailableButtons(GolfGamePython::pyListToVector<Button>(pyButtons));
    }

    pyb::list GolfGamePython::getAvailableButtons(){
        return GolfGamePython::vectorToPyList(this->availableButtons);
    }

    ServerStatePython* GolfGamePython::getServerState() {
        ServerStatePython* pyServerState = new ServerStatePython();

        pyServerState->tic = 1;
        pyServerState->playerCount = 2;

        pyb::list pyPlayersInGame, pyPlayersNames;
        for(int i = 0; i < 2; ++i) {
            pyPlayersInGame.append(i);
            pyPlayersNames.append(pyb::str(std::string("test")));
        }
        
        pyServerState->playersInGame = pyPlayersInGame;
        pyServerState->playersNames = pyPlayersNames;

        return pyServerState;
    }

    template<class T> pyb::list GolfGamePython::vectorToPyList(const std::vector<T>& vector){
        pyb::list pyList;
        for (auto& i : vector) pyList.append(i);

        return pyList;
    }

    template<class T> std::vector<T> GolfGamePython::pyListToVector(pyb::list const &pyList){
        size_t pyLen = pyb::len(pyList);
        std::vector<T> vector = std::vector<T>(pyLen);
        for (size_t i = 0; i < pyLen; ++i) vector[i] = pyb::cast<T>(pyList[i]);

        return vector;
    }

    template<class T> std::vector<T> GolfGamePython::pyArrayToVector(pyb::array_t<T> const &pyArray){
        if (pyArray.ndim() != 1)
            throw std::runtime_error("Number of dimensions larger than 1, should be 1D ndarray");

        size_t pyLen = pyArray.shape(0);
        std::vector<T> vector = std::vector<T>(pyLen);
        for (size_t i = 0; i < pyLen; ++i) vector[i] = pyArray.at(i);

        return vector;
    }

    template<typename T> std::vector<T> GolfGamePython::pyObjectToVector(pyb::object const &pyObject) {
        if(pyb::isinstance<pyb::list>(pyObject) || pyb::isinstance<pyb::tuple>(pyObject))
            return pyListToVector<T>(pyObject);
        else if(pyb::isinstance<pyb::array>(pyObject))
            return pyArrayToVector<T>(pyObject);
        else throw std::runtime_error("Unsupported type, should be list or 1D ndarray of numeric or boolean values");
    }

    template<class T> pyb::array_t<T> GolfGamePython::dataToNumpyArray(std::vector<pyb::ssize_t> dims, T *data){
        return pyb::array(dims, data);
    }

}