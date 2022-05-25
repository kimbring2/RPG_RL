/*
 Copyright (C) 2016 by Wojciech Jaśkowski, Michał Kempka, Grzegorz Runc, Jakub Toczek, Marek Wydmuch

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#ifndef __VIZGOLF_TYPES_H__
#define __VIZGOLF_TYPES_H__

#include "ViZGolfConsts.h"

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace vizgolf {
    typedef std::vector<uint8_t> ImageBuffer;
    typedef std::shared_ptr<ImageBuffer> ImageBufferPtr;

    typedef std::vector<int16_t> AudioBuffer;
    typedef std::shared_ptr<AudioBuffer> AudioBufferPtr;

    struct Label {
        // Label properties
        uint8_t value;
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;

        //bool isObjectHostile; // Right now we do not provide this information
        //bool isObjectPickable;
        //bool isObjectUsable;

        // Actor properties
        unsigned int objectId;

        double objectPositionX;
        double objectPositionY;
        double objectPositionZ;

        double objectAngle;
        double objectPitch;
        double objectRoll;

        double objectVelocityX;
        double objectVelocityY;
        double objectVelocityZ;

        std::string objectName;
    };

    struct Object {
        // Actor properties
        unsigned int id;

        double positionX;
        double positionY;
        double positionZ;

        double angle;
        double pitch;
        double roll;

        double velocityX;
        double velocityY;
        double velocityZ;

        std::string name;
    };

    struct Line{
        double x1;
        double y1;
        double x2;
        double y2;
        bool isBlocking;
    };

    struct Sector{
        double floorHeight;
        double ceilingHeight;
        std::vector<Line> lines;
    };

    struct GameState {
        unsigned int number;
        unsigned int tic;
        double reward;

        std::vector<double> gameVariables;

        ImageBufferPtr screenBuffer;

        std::vector<Label> labels;
        std::vector<Object> objects;
        std::vector<Sector> sectors;
    };

    typedef std::shared_ptr<GameState> GameStatePtr;

    struct ServerState {
        unsigned int tic;
        unsigned int playerCount;
        bool playersInGame[MAX_PLAYERS];
        int playersFrags[MAX_PLAYERS];
        std::string playersNames[MAX_PLAYERS];
        bool playersAfk[MAX_PLAYERS];
        unsigned int playersLastActionTic[MAX_PLAYERS];
        unsigned int playersLastKillTic[MAX_PLAYERS];
    };

    typedef std::shared_ptr<ServerState> ServerStatePtr;

    enum Mode {
        PLAYER,             // synchronous player mode
        SPECTATOR,          // synchronous spectator mode
        ASYNC_PLAYER,       // asynchronous player mode
        ASYNC_SPECTATOR,    // asynchronous spectator mode
    };

    enum ScreenFormat {
        CRCGCB              = 0, // 3 channels of 8-bit values in RGB order
        RGB24               = 1, // channel of RGB values stored in 24 bits, where R value is stored in the oldest 8 bits
        RGBA32              = 2, // channel of RGBA values stored in 32 bits, where R value is stored in the oldest 8 bits
        ARGB32              = 3, // channel of ARGB values stored in 32 bits, where A value is stored in the oldest 8 bits
        CBCGCR              = 4, // 3 channels of 8-bit values in BGR order
        BGR24               = 5, // channel of BGR values stored in 24 bits, where B value is stored in the oldest 8 bits
        BGRA32              = 6, // channel of BGRA values stored in 32 bits, where B value is stored in the oldest 8 bits
        ABGR32              = 7, // channel of ABGR values stored in 32 bits, where A value is stored in the oldest 8 bits
        GRAY8               = 8, // 8-bit gray channel
        DOOM_256_COLORS8    = 9, // 8-bit channel with Doom palette values
    };

    enum ScreenResolution {
        RES_160X120,    // 4:3

        RES_200X125,    // 16:10
        RES_200X150,    // 4:3

        RES_256X144,    // 16:9
        RES_256X160,    // 16:10
        RES_256X192,    // 4:3

        RES_320X180,    // 16:9
        RES_320X200,    // 16:10
        RES_320X240,    // 4:3
        RES_320X256,    // 5:4

        RES_400X225,    // 16:9
        RES_400X250,    // 16:10
        RES_400X300,    // 4:3

        RES_512X288,    // 16:9
        RES_512X320,    // 16:10
        RES_512X384,    // 4:3

        RES_640X360,    // 16:9
        RES_640X400,    // 16:10
        RES_640X480,    // 4:3

        RES_800X450,    // 16:9
        RES_800X500,    // 16:10
        RES_800X600,    // 4:3

        RES_1024X576,   // 16:9
        RES_1024X640,   // 16:10
        RES_1024X768,   // 4:3

        RES_1280X720,   // 16:9
        RES_1280X800,   // 16:10
        RES_1280X960,   // 4:3
        RES_1280X1024,  // 5:4

        RES_1400X787,   // 16:9
        RES_1400X875,   // 16:10
        RES_1400X1050,  // 4:3

        RES_1600X900,   // 16:9
        RES_1600X1000,  // 16:10
        RES_1600X1200,  // 4:3

        RES_1920X1080,  // 16:9
    };

    enum AutomapMode {
        NORMAL,             // Only level architecture the player has seen is shown.
        WHOLE,              // All architecture is shown, regardless of whether or not the player has seen it.
        OBJECTS,            // In addition to the previous, shows all things in the map as arrows pointing in
                            // the direction they are facing.
        OBJECTS_WITH_SIZE,  // In addition to the previous, all things are wrapped in a box showing their size.
    };

    enum SamplingRate {
        SR_11025,           // Low sampling rate
        SR_22050,           // Medium sampling rate.
        SR_44100,           // High sampling rate
    };

    enum GameVariable {
        /* Defined variables */
        // STROKE
        STROKE_0,
        STROKE_1,

        // LEVEL
        LEVEL_0,
        LEVEL_1,
    };

    enum Button {
        /* Binary buttons */
        ATTACK          = 0,
        USE             = 1,
        JUMP            = 2,
        CROUCH          = 3,
        TURN180         = 4,
        ALTATTACK       = 5,
        RELOAD          = 6,
        ZOOM            = 7,

        SPEED           = 8,
        STRAFE          = 9,

        MOVE_RIGHT      = 10,
        MOVE_LEFT       = 11,
        MOVE_BACKWARD   = 12,
        MOVE_FORWARD    = 13,
        TURN_RIGHT      = 14,
        TURN_LEFT       = 15,
        LOOK_UP         = 16,
        LOOK_DOWN       = 17,
        MOVE_UP         = 18,
        MOVE_DOWN       = 19,
        LAND            = 20,

        SELECT_WEAPON1  = 21,
        SELECT_WEAPON2  = 22,
        SELECT_WEAPON3  = 23,
        SELECT_WEAPON4  = 24,
        SELECT_WEAPON5  = 25,
        SELECT_WEAPON6  = 26,
        SELECT_WEAPON7  = 27,
        SELECT_WEAPON8  = 28,
        SELECT_WEAPON9  = 29,
        SELECT_WEAPON0  = 30,

        SELECT_NEXT_WEAPON          = 31,
        SELECT_PREV_WEAPON          = 32,
        DROP_SELECTED_WEAPON        = 33,

        ACTIVATE_SELECTED_ITEM      = 34,
        SELECT_NEXT_ITEM            = 35,
        SELECT_PREV_ITEM            = 36,
        DROP_SELECTED_ITEM          = 37,

        /* Delta buttons */
        LOOK_UP_DOWN_DELTA          = 38,
        TURN_LEFT_RIGHT_DELTA       = 39,
        MOVE_FORWARD_BACKWARD_DELTA = 40,
        MOVE_LEFT_RIGHT_DELTA       = 41,
        MOVE_UP_DOWN_DELTA          = 42,
    };

}
#endif
