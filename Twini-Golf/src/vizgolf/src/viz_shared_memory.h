#ifndef __VIZ_SHARED_MEMORY_H__
#define __VIZ_SHARED_MEMORY_H__

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/current_function.hpp>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define VIZ_SM_NAME_BASE "ViZGolfSM"
#define VIZ_SM_REGION_COUNT 7

#define VIZ_SM_GAMESTATE_NUM    0
#define VIZ_SM_INPUTSTATE_NUM   1
#define VIZ_SM_SCREEN_NUM       2
#define VIZ_SM_DEPTH_NUM        3
#define VIZ_SM_LABELS_NUM       4
#define VIZ_SM_AUTOMAP_NUM      5
#define VIZ_SM_AUDIO_NUM        6

#define VIZ_SM_GAMESTATE    vizSMRegion[VIZ_SM_GAMESTATE_NUM]
#define VIZ_SM_INPUTSTATE   vizSMRegion[VIZ_SM_INPUTSTATE_NUM]
#define VIZ_SM_SCREEN       vizSMRegion[VIZ_SM_SCREEN_NUM]
#define VIZ_SM_DEPTH        vizSMRegion[VIZ_SM_DEPTH_NUM]
#define VIZ_SM_LABELS       vizSMRegion[VIZ_SM_LABELS_NUM]
#define VIZ_SM_AUTOMAP      vizSMRegion[VIZ_SM_AUTOMAP_NUM]
#define VIZ_SM_AUDIO        vizSMRegion[VIZ_SM_AUDIO_NUM]

#define VIZ_TIME                    ((unsigned int)level.starttime + (unsigned int)level.maptime)

#define VIZ_PLAYER_NUM              consoleplayer
#define VIZ_PLAYER                  players[consoleplayer]
#define VIZ_MAX_PLAYERS             16 // 16
#define VIZ_MAX_PLAYER_NAME_LEN     127 + 1 // (127 + 1 = 128, old: 15 + 1 = 16)
#define VIZ_MAX_NAME_LEN            VIZ_MAX_PLAYER_NAME_LEN

#define VIZ_FUNC            BOOST_CURRENT_FUNCTION

#define VIZ_MAX_ERROR_TEXT_LEN      128
#define VIZ_MAX_DEBUG_TEXT_LEN      1024

#define VIZ_MAX_LABELS              256
#define VIZ_MAX_OBJECTS             4096
#define VIZ_MAX_SECTORS             2048
#define VIZ_MAX_LINES               8192

#define VIZ_GV_USER_COUNT 60
#define VIZ_GV_SLOTS_SIZE 10

#define VIZ_BT_CMD_BT_COUNT         38
#define VIZ_BT_AXIS_BT_COUNT        5
#define VIZ_BT_COUNT                43

#define VIZ_VERSION                 1113
#define VIZ_VERSION_STR             "1.1.13"

typedef int8_t                  SBYTE;
typedef uint8_t                 BYTE;
typedef int16_t                 SWORD;
typedef uint16_t                WORD;
typedef int32_t                 SDWORD;
typedef uint32_t                uint32;
typedef int64_t                 SQWORD;
typedef uint64_t                QWORD;

namespace bip = boost::interprocess;

extern bip::shared_memory_object vizSM;
extern size_t vizSMSize;
extern unsigned int vizScreenWidth, vizScreenHeight;
extern size_t vizScreenPitch, vizScreenSize, vizScreenChannelSize;


enum VIZScreenFormat {
    VIZ_SCREEN_CRCGCB           = 0,
    VIZ_SCREEN_RGB24            = 1,
    VIZ_SCREEN_RGBA32           = 2,
    VIZ_SCREEN_ARGB32           = 3,
    VIZ_SCREEN_CBCGCR           = 4,
    VIZ_SCREEN_BGR24            = 5,
    VIZ_SCREEN_BGRA32           = 6,
    VIZ_SCREEN_ABGR32           = 7,
    VIZ_SCREEN_GRAY8            = 8,
    VIZ_SCREEN_DOOM_256_COLORS8 = 9
};


struct VIZSMRegion {
    bip::mapped_region *region;
    void *address;
    size_t offset;
    size_t size;
    bool writeable;

    VIZSMRegion(){
        this->region = NULL;
        this->address = NULL;
        this->offset = 0;
        this->size = 0;
        this->writeable = false;
    };
};


// These structures should reflect structures in lib/ViZDoomSharedMemory.h
struct VIZLabel {
    BYTE value;
    unsigned int position[2];
    unsigned int size[2];

    unsigned int objectId;
    double objectPosition[9];
    char objectName[VIZ_MAX_NAME_LEN];
};


struct VIZGameState {
    // BALL
    unsigned int BALL_STROKE_0;
    unsigned int BALL_STROKE_1;

    // LEVEL
    unsigned int LEVEL_0;
    unsigned int LEVEL_1;

    // LABELS
    unsigned int LABEL_COUNT;
    VIZLabel LABEL[VIZ_MAX_LABELS];

    // REWARD
    double REWARD;

    // SM
    size_t SM_SIZE;
    size_t SM_REGION_OFFSET[VIZ_SM_REGION_COUNT];
    size_t SM_REGION_SIZE[VIZ_SM_REGION_COUNT];
    bool SM_REGION_WRITEABLE[VIZ_SM_REGION_COUNT];
};


extern VIZSMRegion vizSMRegion[VIZ_SM_REGION_COUNT];

void VIZ_SMInit(const char * id);

void VIZ_SMUpdate(size_t buffersSize);

void VIZ_SMCreateRegion(VIZSMRegion* regionPtr, bool writeable, size_t offset, size_t size);

void VIZ_SMDeleteRegion(VIZSMRegion* regionPtr);

size_t VIZ_SMGetRegionOffset(VIZSMRegion* regionPtr);

void VIZ_SMClose();

void VIZ_GameStateInit();

void VIZ_GameStateUpdateStrokes(unsigned int ball_stroke_0, unsigned int ball_stroke_1);

void VIZ_GameStateUpdateLevel(unsigned int level_0, unsigned int level_1);

void VIZ_GameStateUpdateReward(unsigned int reward);

void VIZ_GameStateUpdateVariables(int map_killcount);

void VIZ_GameStateSMUpdate();

//void VIZ_Error(const char *func, const char *error, ...);

//void VIZ_DebugMsg(int level, const char *func, const char *msg, ...);


enum VIZButton{
    VIZ_BT_ATTACK           = 0,
    VIZ_BT_USE              = 1,
    VIZ_BT_JUMP             = 2,
    VIZ_BT_CROUCH           = 3,
    VIZ_BT_TURN180          = 4,
    VIZ_BT_ALTATTACK        = 5,
    VIZ_BT_RELOAD           = 6,
    VIZ_BT_ZOOM             = 7,

    VIZ_BT_SPEED            = 8,
    VIZ_BT_STRAFE           = 9,

    VIZ_BT_MOVE_RIGHT       = 10,
    VIZ_BT_MOVE_LEFT        = 11,
    VIZ_BT_MOVE_BACK        = 12,
    VIZ_BT_MOVE_FORWARD     = 13,
    VIZ_BT_TURN_RIGHT       = 14,
    VIZ_BT_TURN_LEFT        = 15,
    VIZ_BT_LOOK_UP          = 16,
    VIZ_BT_LOOK_DOWN        = 17,
    VIZ_BT_MOVE_UP          = 18,
    VIZ_BT_MOVE_DOWN        = 19,
    VIZ_BT_LAND             = 20,

    VIZ_BT_SELECT_WEAPON1   = 21,
    VIZ_BT_SELECT_WEAPON2   = 22,
    VIZ_BT_SELECT_WEAPON3   = 23,
    VIZ_BT_SELECT_WEAPON4   = 24,
    VIZ_BT_SELECT_WEAPON5   = 25,
    VIZ_BT_SELECT_WEAPON6   = 26,
    VIZ_BT_SELECT_WEAPON7   = 27,
    VIZ_BT_SELECT_WEAPON8   = 28,
    VIZ_BT_SELECT_WEAPON9   = 29,
    VIZ_BT_SELECT_WEAPON0   = 30,

    VIZ_BT_SELECT_NEXT_WEAPON       = 31,
    VIZ_BT_SELECT_PREV_WEAPON       = 32,
    VIZ_BT_DROP_SELECTED_WEAPON     = 33,

    VIZ_BT_ACTIVATE_SELECTED_ITEM   = 34,
    VIZ_BT_SELECT_NEXT_ITEM         = 35,
    VIZ_BT_SELECT_PREV_ITEM         = 36,
    VIZ_BT_DROP_SELECTED_ITEM       = 37,

    VIZ_BT_VIEW_PITCH_AXIS          = 38,
    VIZ_BT_VIEW_ANGLE_AXIS          = 39,
    VIZ_BT_FORWARD_BACKWARD_AXIS    = 40,
    VIZ_BT_LEFT_RIGHT_AXIS          = 41,
    VIZ_BT_UP_DOWN_AXIS             = 42,
};

#define VIZ_BT_CMD_BT_COUNT         38
#define VIZ_BT_AXIS_BT_COUNT        5
#define VIZ_BT_COUNT                43


struct VIZInputState {
    double BT[VIZ_BT_COUNT];
    bool BT_AVAILABLE[VIZ_BT_COUNT];
    double BT_MAX_VALUE[VIZ_BT_AXIS_BT_COUNT];
    double CMD_BT[VIZ_BT_COUNT]; // Parsed cmd->ucmd.buttons
};


void VIZ_InputInit();
void VIZ_ScreenInit();
void VIZ_GetInputState();
void VIZ_ScreenBufferUpdate(unsigned char *screen_buffer);

#endif
