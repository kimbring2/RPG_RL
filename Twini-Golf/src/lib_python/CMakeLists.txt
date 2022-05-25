find_package(PythonInterp 3 REQUIRED)


set(PYTHON_VERSION_FULL ${PYTHON_VERSION_STRING})
STRING(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" PYTHON_VERSION_MINOR ${PYTHON_VERSION_STRING})
find_package(PythonLibs ${PYTHON_VERSION_MINOR} REQUIRED)

add_subdirectory(pybind11)
link_directories(${PROJECT_SOURCE_DIR})

include_directories(${VIZDOOM_PYTHON_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall -m64 -I include")
set(VIZGOLF_PYTHON_SRC_DIR ${VIZGOLF_SRC_DIR}/lib_python)
set(VIZGOLF_PYTHON_INCLUDE_DIR ${VIZGOLF_INCLUDE_DIR} ${VIZGOLF_PYTHON_SRC_DIR})
set(VIZGOLF_PYTHON_OUTPUT_DIR ${VIZGOLF_OUTPUT_DIR}/python${PYTHON_VERSION_MINOR})

set(VIZGOLF_PYTHON_SOURCES
    ${VIZGOLF_PYTHON_SRC_DIR}/ViZGolfGamePython.h
    ${VIZGOLF_PYTHON_SRC_DIR}/ViZGolfGamePython.cpp
    ${VIZGOLF_PYTHON_SRC_DIR}/ViZGolfPythonModule.cpp)

set(VIZGOLF_PYTHON_LIBS
    ${VIZGOLF_LIBS}
    ${Boost_LIBRARIES})


MESSAGE(STATUS "Boost_LIBRARIES:" ${Boost_LIBRARIES})

add_library(golf_game SHARED ${VIZGOLF_LIB_SRC_DIR}/ViZGolfGame.cpp)
add_library(golf_controller SHARED ${VIZGOLF_LIB_SRC_DIR}/ViZGolfController.cpp)
add_library(golf_shared_memory SHARED ${VIZGOLF_LIB_SRC_DIR}/ViZGolfSharedMemory.cpp)
add_library(golf_exception SHARED ${VIZGOLF_LIB_SRC_DIR}/ViZGolfExceptions.cpp)

MESSAGE(STATUS "VIZDOOM_PYTHON_INCLUDE_DIR:" ${VIZDOOM_PYTHON_INCLUDE_DIR})

pybind11_add_module(vizgolf ${VIZGOLF_PYTHON_SOURCES})
target_link_libraries(vizgolf PUBLIC golf_game golf_controller golf_shared_memory golf_exception libvizgolf_static ${VIZGOLF_PYTHON_LIBS})