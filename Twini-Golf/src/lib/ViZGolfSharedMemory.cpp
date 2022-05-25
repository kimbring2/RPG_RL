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

#include "ViZGolfSharedMemory.h"
#include "ViZGolfExceptions.h"


namespace vizgolf {
    SharedMemory::SharedMemory(std::string name) : name(name) {
        this->init();
    }

    SharedMemory::~SharedMemory() {
        this->close();
    }

    void SharedMemory::init() {
        try {
            printf("this->name.c_str(): %s\n", this->name.c_str());
            this->sm = bip::shared_memory_object(bip::open_only, this->name.c_str(), bip::read_write);
        }
        catch(bip::interprocess_exception& ex) {
            throw SharedMemoryException(std::string("Failed to open shared memory: ") + std::string(ex.what()));
        }
        catch (...) {
            throw SharedMemoryException("Failed to open shared memory for unknown reason.");
        }

        this->update();
    }

    void SharedMemory::update() {
        try {
            this->sm.get_size(this->size);

            for (int i = 0; i < SM_REGION_COUNT; ++i) {
                SMRegion *regionPtr = &this->region[i];

                if (i == 0 && !regionPtr->address) {
                    regionPtr->offset = 0;
                    regionPtr->size = sizeof(SMGameState);
                    regionPtr->writeable = false;
                    this->mapRegion(regionPtr);
                } else if (!regionPtr->address ||
                           (regionPtr->offset != this->getGameState()->SM_REGION_OFFSET[i]
                            && regionPtr->size != this->getGameState()->SM_REGION_SIZE[i])) {
                    this->deleteRegion(regionPtr);

                    regionPtr->offset = this->getGameState()->SM_REGION_OFFSET[i];
                    regionPtr->size = this->getGameState()->SM_REGION_SIZE[i];
                    regionPtr->writeable = this->getGameState()->SM_REGION_WRITEABLE[i];

                    this->mapRegion(regionPtr);
                }
            }
        }
        catch(bip::interprocess_exception& ex) {
            throw SharedMemoryException(std::string("Failed to map shared memory regions: ") + std::string(ex.what()));
        }
        catch (...) {
            throw SharedMemoryException("Failed to map shared memory regions for unknown reason.");
        }

        //printf("this->getGameState()->BALL_STROKE_0: %d\n", this->getGameState()->BALL_STROKE_0);
        //printf("this->getGameState()->BALL_STROKE_1: %d\n", this->getGameState()->BALL_STROKE_1);
        //printf("this->getGameState()->SM_REGION_SIZE[0]: %d\n", this->getGameState()->SM_REGION_SIZE[0]);
        //printf("this->getGameState()->SM_REGION_SIZE[1]: %d\n", this->getGameState()->SM_REGION_SIZE[1]);

        //printf("this->size: %d\n", this->size);
        //printf("sizeof(SMGameState): %d\n", sizeof(SMGameState));
        //printf("sizeof(SMInputState): %d\n", sizeof(SMInputState));
        //printf("this->getGameState()->MAP_KILLCOUNT: %d\n", this->getGameState()->MAP_KILLCOUNT);

        //printf("")
        /*
        if (this->getGameState()->SM_SIZE != this->size ||
            this->getGameState()->SM_REGION_SIZE[0] != sizeof(SMGameState) ||
            this->getGameState()->SM_REGION_SIZE[1] != sizeof(SMInputState))
            throw SharedMemoryException(
                    "Memory size does not match the the expected size. Possible ViZDoom version mismatch.");
        */
    }

    void SharedMemory::mapRegion(SMRegion *regionPtr) {
        if (regionPtr->size) {
            regionPtr->region = new bip::mapped_region(this->sm,
                                                       regionPtr->writeable ? bip::read_write : bip::read_only,
                                                       regionPtr->offset, regionPtr->size);

            regionPtr->address = regionPtr->region->get_address();
        }
    }

    void SharedMemory::deleteRegion(SMRegion *regionPtr) {
        if (regionPtr->region) {
            delete regionPtr->region;
            regionPtr->region = nullptr;
            regionPtr->address = nullptr;
            regionPtr->size = 0;
        }
    }

    void SharedMemory::close() {
        for (int i = 0; i < SM_REGION_COUNT; ++i) this->deleteRegion(&this->region[i]);
        bip::shared_memory_object::remove(this->name.c_str());
    }

    SMGameState *SharedMemory::getGameState() {
        return static_cast<SMGameState *>(this->region[0].address);
    }

    SMInputState *SharedMemory::getInputState() {
        return static_cast<SMInputState *>(this->region[1].address);
    }

    uint8_t *SharedMemory::getScreenBuffer() {
        return static_cast<uint8_t *>(this->region[2].address);
    }
}