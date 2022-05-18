//#include "../vizgolf/Ball.h"
//#include "../vizgolf/Entity.h"
//#include "../src/Hole.h"
//#include "../vizgolf/Math.h"
//#include "../vizgolf/RenderWindow.h"
//#include "../vizgolf/Tile.h"

//#include "ViZGolf.h"
#include "ViZGolfGamePython.h"

#include "pybind11/include/pybind11/pybind11.h"

// to convert C++ STL containers to python list
#include "pybind11/include/pybind11/stl.h" 

namespace py = pybind11;
using namespace std;

PYBIND11_MODULE(golf, vz)
{
    py::class_<vizgolf::GolfGamePython>(vz, "GolfGame")
        .def(py::init<>())
        .def("init", &vizgolf::GolfGamePython::init)
        .def("launchGolf", &vizgolf::GolfGamePython::launchGolf)
        .def("generateSharedMemory", &vizgolf::GolfGamePython::generateSharedMemory)
        .def("getValueFromSharedMemory", &vizgolf::GolfGamePython::getValueFromSharedMemory)
        .def("generateMessageQueue", &vizgolf::GolfGamePython::generateMessageQueue)
        .def("sendCommand", &vizgolf::GolfGamePython::sendCommand);
}

