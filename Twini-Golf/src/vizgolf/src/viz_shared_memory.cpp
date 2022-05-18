#include "viz_shared_memory.h"

VIZGameState *vizGameStateSM = NULL;

bip::shared_memory_object vizSM;
size_t vizSMSize;
//char * vizSMName;
const char * vizSMName;

unsigned int vizScreenWidth, vizScreenHeight;
size_t vizScreenPitch, vizScreenSize, vizScreenChannelSize;
size_t vizAudioSamplesPerTic, vizAudioSizePerTic, vizAudioSize;

int posMulti, rPos, gPos, bPos, aPos;
bool alpha;

BYTE *vizScreenSM = NULL, *vizDepthSM = NULL, *vizLabelsSM = NULL, *vizAutomapSM = NULL, *vizAudioSM = NULL;

VIZSMRegion vizSMRegion[VIZ_SM_REGION_COUNT];


VIZInputState *vizInput = NULL;
bool vizInputInited = false;
float vizLastInputBT[VIZ_BT_COUNT];
unsigned int vizLastInputUpdate[VIZ_BT_COUNT];


void VIZ_SMInit(const char * id){
    printf("VIZ_SMInit: Init shared memory.\n");

    //vizSMName = new char[strlen(VIZ_SM_NAME_BASE) + strlen(id) + 1];
    //vizSMName = new char[strlen(id)];
    //strcpy(vizSMName, VIZ_SM_NAME_BASE);
    //strcat(vizSMName, id);
    vizSMName = id;

    try {
        printf("vizSMName : %s\n", vizSMName);

        bip::shared_memory_object::remove(vizSMName);
        vizSM = bip::shared_memory_object(bip::open_or_create, vizSMName, bip::read_write);

        vizSMSize = sizeof(VIZGameState) + sizeof(VIZInputState);
        vizSM.truncate(vizSMSize);

        //VIZ_DebugMsg(1, VIZ_FUNC, "SMName: %s, SMSize: %zu", vizSMName, vizSMSize);
    }
    catch(...){ // bip::interprocess_exception
        //VIZ_Error(VIZ_FUNC, "Failed to create shared memory.");
        printf("Failed to create shared memory.");
        throw;
    }
}


void VIZ_SMUpdate(size_t buffersSize){
    try {
        vizSMSize = sizeof(VIZGameState) + sizeof(VIZInputState) + buffersSize;
        vizSM.truncate(vizSMSize);

        //VIZ_DebugMsg(3, VIZ_FUNC, "New SMSize: %zu", vizSMSize);
    }
    catch(...){ // bip::interprocess_exception
        //VIZ_Error(VIZ_FUNC, "Failed to truncate shared memory.");
        throw;
    }
}


void VIZ_SMCreateRegion(VIZSMRegion* regionPtr, bool writeable, size_t offset, size_t size){
    regionPtr->offset = offset;
    regionPtr->size = size;
    regionPtr->writeable = writeable;
    if (regionPtr->size) {
        regionPtr->region = new bip::mapped_region(vizSM, bip::read_write, offset, size);
        regionPtr->address = regionPtr->region->get_address();
    }
}


void VIZ_SMDeleteRegion(VIZSMRegion* regionPtr) {
    if(regionPtr->region){
        delete regionPtr->region;
        regionPtr->region = NULL;
        regionPtr->address = NULL;
        regionPtr->size = 0;
        regionPtr->offset = 0;
        regionPtr->writeable = false;
    }
}


size_t VIZ_SMGetRegionOffset(VIZSMRegion* regionPtr) {
    size_t offset = 0;
    for (auto i = &vizSMRegion[0]; i != regionPtr; ++i) offset += i->size;
    return offset;
}


void VIZ_SMClose() {
    for (int i = 0; i < VIZ_SM_REGION_COUNT; ++i) VIZ_SMDeleteRegion(&vizSMRegion[i]);
	delete[] vizSMName;
}


/* Main functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void VIZ_GameStateInit() {
    printf("VIZ_GameStateInit\n");

    try {
        VIZSMRegion* gameStateRegion = &VIZ_SM_GAMESTATE;
        VIZ_SMCreateRegion(gameStateRegion, false, 0, sizeof(VIZGameState));
        vizGameStateSM = static_cast<VIZGameState *>(gameStateRegion->address);

        //VIZ_DebugMsg(1, VIZ_FUNC, "gameStateOffset: %zu, gameStateSize: %zu", gameStateRegion->offset,
        //             sizeof(VIZGameState));
    }
    catch(bip::interprocess_exception &ex){
        //VIZ_Error(VIZ_FUNC, "Failed to create game state.");
        printf("bip::interprocess_exception &ex");
        throw;
    }

    vizGameStateSM->LEVEL_0 = 1;
    vizGameStateSM->LEVEL_1 = 1;
    vizGameStateSM->BALL_STROKE_0 = 0;
    vizGameStateSM->BALL_STROKE_1 = 0;

    vizGameStateSM->BALL_POS_0.x = 0;
    vizGameStateSM->BALL_POS_0.y = 0;

    vizGameStateSM->BALL_POS_1.x = 0;
    vizGameStateSM->BALL_POS_1.y = 0;

    vizGameStateSM->MAP_KILLCOUNT = 12345;

    /*
    vizGameStateSM->VERSION = VIZ_VERSION;
    strncpy(vizGameStateSM->VERSION_STR, VIZ_VERSION_STR, 8);
    vizGameStateSM->SM_SIZE = vizSMSize;

    vizGameStateSM->GAME_TIC = 0;
    vizGameStateSM->PLAYER_HEALTH = 0;
    vizGameStateSM->PLAYER_HAS_ACTOR = false;
    vizGameStateSM->PLAYER_DEAD = true;

    for (int i = 0; i < 9; ++i) {
        //printf("i : %d\n", i);
        vizGameStateSM->PLAYER_MOVEMENT[i] = 0;
    }
    */
}


void VIZ_GameStateSMUpdate(){
    printf("VIZ_GameStateSMUpdate\n");

    if(!vizGameStateSM) return;

    vizGameStateSM->SM_SIZE = vizSMSize;

    //vizGameStateSM->SCREEN_WIDTH = vizScreenWidth;
    //vizGameStateSM->SCREEN_HEIGHT = vizScreenHeight;
    //vizGameStateSM->SCREEN_PITCH = vizScreenPitch;
    //vizGameStateSM->SCREEN_SIZE = vizScreenSize;
    //vizGameStateSM->SCREEN_FORMAT = *viz_screen_format;

    //vizGameStateSM->DEPTH_BUFFER = *viz_depth && vizDepthMap;
    //vizGameStateSM->LABELS = *viz_labels && vizLabels;
    //vizGameStateSM->AUTOMAP = *viz_automap;
    //vizGameStateSM->OBJECTS = *viz_objects;
    //vizGameStateSM->SECTORS = *viz_sectors;

    for(int i = 0; i < VIZ_SM_REGION_COUNT; ++i){
        //printf("i: %d\n", i);
        //printf("vizSMRegion[i].size: %d\n", vizSMRegion[i].size);
        //printf("\n");
        vizGameStateSM->SM_REGION_OFFSET[i] = vizSMRegion[i].offset;
        vizGameStateSM->SM_REGION_SIZE[i] = vizSMRegion[i].size;
        vizGameStateSM->SM_REGION_WRITEABLE[i] = vizSMRegion[i].writeable;
    }
}


void VIZ_GameStateUpdateStrokes(unsigned int ball_stroke_0, unsigned int ball_stroke_1) {
    //printf("VIZ_GameStateUpdateVariables\n");
    vizGameStateSM->BALL_STROKE_0 = ball_stroke_0;
    vizGameStateSM->BALL_STROKE_1 = ball_stroke_1;
}


void VIZ_GameStateUpdateLevel(unsigned int level_0, unsigned int level_1) {
    //printf("VIZ_GameStateUpdateVariables\n");
    vizGameStateSM->LEVEL_0 = level_0;
    vizGameStateSM->LEVEL_1 = level_1;
}


void VIZ_GameStateUpdateBallPos(BALLPos ball_pos_0, BALLPos ball_pos_1) {
    //printf("VIZ_GameStateUpdateVariables\n");
    vizGameStateSM->BALL_POS_0 = ball_pos_0;
    vizGameStateSM->BALL_POS_1 = ball_pos_1;
}


void VIZ_GameStateUpdateVariables(int map_killcount) {
    //printf("VIZ_GameStateUpdateVariables\n");
    //printf("map_killcount: %d\n", map_killcount);
    int max = 100;
    int min = 1;

    int output = min + (rand() % static_cast<int>(max - min + 1));
    printf("output: %d\n", output);
    vizGameStateSM->MAP_KILLCOUNT = output;
}


void VIZ_InputInit() {
    try {
        printf("VIZ_InputInit\n");
        VIZSMRegion* inputRegion = &VIZ_SM_INPUTSTATE;
        VIZ_SMCreateRegion(inputRegion, true, VIZ_SMGetRegionOffset(inputRegion), sizeof(VIZInputState));
        vizInput = static_cast<VIZInputState *>(inputRegion->address);
    }
    catch (bip::interprocess_exception &ex) {
        throw;
    }

    for (size_t i = 0; i < VIZ_BT_COUNT; ++i) {
        vizInput->BT[i] = 0;
        vizInput->BT_AVAILABLE[i] = true;
        vizLastInputBT[i] = 0;
        vizLastInputUpdate[i] = 0;
    }

    for(size_t i = 0; i < VIZ_BT_AXIS_BT_COUNT; ++i){
        vizInput->BT_MAX_VALUE[i] = 0;
    }
}