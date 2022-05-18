//#include "../vizgolf/Ball.h"
//#include "../vizgolf/Entity.h"
#include "../src/Hole.h"
//#include "../vizgolf/Math.h"
//#include "../vizgolf/RenderWindow.h"
//#include "../vizgolf/Tile.h"

//#include "ViZGolf.h"
//#include "ViZGolfGamePython.h"

#include "../pybind11/include/pybind11/pybind11.h"

// to convert C++ STL containers to python list
#include "../pybind11/include/pybind11/stl.h" 

namespace py = pybind11;
using namespace std;
PYBIND11_MODULE(vizgolf_, vz)
{
    py::class_<ISpeech>(m, "ISpeech");

    // Add the base class to work polymorphism.
    // For example T800 constructed with ISpeech, if
    // we don't declare it here, python doesn't allow
    // injectign SpeechV1 to T800 constructor.
    py::class_<SpeechV1, ISpeech>(m, "SpeechV1")
        .def(py::init<string>()); //Constructor

    py::class_<SpeechV2, ISpeech>(m, "SpeechV2")
        .def(py::init<string>()); // Constructor

    py::class_<IRobot>(m, "IRobot"); // Abstract, do not add constructor
    
    py::class_<T800, IRobot>(m, "T800")
        .def(py::init<string, int, ISpeech &>()) // constructor
        .def("Walk", &T800::Walk)
        .def("Talk", &T800::Talk)
        .def("GetData", &T800::GetData)
        // read-write public data memeber
        // you can use def_readonly as well.
        .def_readwrite("year", &T800::year); 

    using T = double;
    py::class_<T1000<T>, IRobot>(m, "T1000")
        .def(py::init<string, T, ISpeech &>()) // constructor
        .def("Walk", &T1000<T>::Walk) // method
        .def("Talk", &T1000<T>::Talk) // method
        // Define property with getter and setter
        .def_property("height", &T1000<T>::GetHeight,&T1000<T>::SetHeight); 

    m.def("Move", &Move);
}

