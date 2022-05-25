cmake_minimum_required(VERSION 3.13)
project(twini_golf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall -m64 -I include")

file(GLOB helloworld_SRC
     "src/*.h"
     "src/*.cpp"
)

set(OpenCV_DIR /home/kimbring2/opencv/build)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# always
FIND_PACKAGE(Threads REQUIRED)

# if using boost
SET(Boost_USE_MULTITHREADED ON)

set(prefix "/usr")
set(exec_prefix "${prefix}")
set(libdir "/usr/lib64")
set(SDL2_PREFIX "/usr")
set(SDL2_EXEC_PREFIX "/usr")
set(SDL2_LIBDIR "/usr/lib64")
set(SDL2_INCLUDE_DIRS "${prefix}/include/SDL2")
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer")
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

add_executable(twingolf ${helloworld_SRC})
target_link_libraries(twingolf ${SDL2_LIBRARIES} rt ${OpenCV_LIBS} ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})