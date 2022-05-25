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

#ifndef __VIZGOLF_SHAREDMEMORY_H__
#define __VIZGOLF_SHAREDMEMORY_H__

#include "ViZGolfConsts.h"
#include "ViZGolfTypes.h"

#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <cstdint>

#define SM_REGION_COUNT 7

#define MAX_LABELS 256
#define MAX_OBJECTS 4096
#define MAX_SECTORS 2048
#define MAX_LINES 8192


namespace vizgolf {
    namespace b         = boost;
    namespace bip       = boost::interprocess;

    /* SM region */
    /*----------------------------------------------------------------------------------------------------------------*/
    struct SMRegion {
        bip::mapped_region *region;
        void *address;
        size_t offset;
        size_t size;
        bool writeable;

        SMRegion() {
            this->region = nullptr;
            this->address = nullptr;
            this->offset = 0;
            this->size = 0;
            this->writeable = false;
        };
    };

    /* SM structs */
    /*----------------------------------------------------------------------------------------------------------------*/
    struct SMLabel {
        uint8_t value;
        unsigned int position[2];
        unsigned int size[2];

        unsigned int objectId;
        double objectPosition[9];
        char objectName[MAX_NAME_LENGTH];
    };

    struct SMObject {
        unsigned int id;
        double position[9];
        char name[MAX_NAME_LENGTH];
    };

    struct SMSector{
        double floorHeight;
        double ceilingHeight;
        unsigned int lineCount;
        unsigned int lines[128];
    };

    struct SMLine{
        double position[4];
        bool isBlocking;
    };

    struct SMGameState {
        // BALL
        unsigned int BALL_STROKE_0;
        unsigned int BALL_STROKE_1;

        // LEVEL
        unsigned int LEVEL_0;
        unsigned int LEVEL_1;

        // LABELS
        unsigned int LABEL_COUNT;
        SMLabel LABEL[MAX_LABELS];

        // REWARD
        double REWARD;

        // SM
        size_t SM_SIZE;
        size_t SM_REGION_OFFSET[SM_REGION_COUNT];
        size_t SM_REGION_SIZE[SM_REGION_COUNT];
        bool SM_REGION_WRITEABLE[SM_REGION_COUNT];
    };

    struct SMInputState {
        double BT[BUTTON_COUNT];
        bool BT_AVAILABLE[BUTTON_COUNT];
        double BT_MAX_VALUE[DELTA_BUTTON_COUNT];
        double CMD_BT[BUTTON_COUNT];
    };

    /* SM class */
    /*----------------------------------------------------------------------------------------------------------------*/
    class SharedMemory {

    public:
        SharedMemory(std::string name);
        ~SharedMemory();

        void init();
        void update();
        void close();

        SMGameState *getGameState();
        SMInputState *getInputState();
        uint8_t *getScreenBuffer();

    private:
        bip::shared_memory_object sm;
        bip::offset_t size;
        std::string name;

        void mapRegion(SMRegion *regionPtr);
        void deleteRegion(SMRegion *regionPtr);

        //0 - GameState, 1 - InputState, 2 - ScreenBuffer, 3 - DepthBuffer, 4 - LabelsBuffer, 5 - AutomapBuffer, 6 - AudioBuffer
        SMRegion region[7];
    };
}

#endif
