#include "viz_shared_memory.h"

using namespace cv;

VIZGameState *vizGameStateSM = NULL;

bip::shared_memory_object vizSM;
size_t vizSMSize;
//char * vizSMName;
const char * vizSMName;

int posMulti, rPos, gPos, bPos, aPos;
bool alpha;

BYTE *vizScreenSM = NULL;

size_t vizScreenPitch, vizScreenSize, vizScreenChannelSize;

VIZSMRegion vizSMRegion[VIZ_SM_REGION_COUNT];

VIZInputState *vizInput = NULL;
bool vizInputInited = false;
float vizLastInputBT[VIZ_BT_COUNT];
unsigned int vizLastInputUpdate[VIZ_BT_COUNT];


void VIZ_SMInit(const char * id){
    //printf("VIZ_SMInit: Init shared memory.\n");

    vizSMName = new char[strlen(VIZ_SM_NAME_BASE) + strlen(id) + 1];
    strcpy((char*) vizSMName, VIZ_SM_NAME_BASE);
    strcat((char*) vizSMName, id);

    try {
        bip::shared_memory_object::remove(vizSMName);
        vizSM = bip::shared_memory_object(bip::open_or_create, vizSMName, bip::read_write);

        vizSMSize = sizeof(VIZGameState) + sizeof(VIZInputState);
        vizSM.truncate(vizSMSize);
    }
    catch(...){ // bip::interprocess_exception
        //printf("Failed to create shared memory.");
        throw;
    }
}


void VIZ_SMUpdate(size_t buffersSize){
    try {
        vizSMSize = sizeof(VIZGameState) + sizeof(VIZInputState) + buffersSize;
        vizSM.truncate(vizSMSize);
    }
    catch(...){ // bip::interprocess_exception
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
    //printf("VIZ_GameStateInit\n");

    try {
        VIZSMRegion* gameStateRegion = &VIZ_SM_GAMESTATE;
        VIZ_SMCreateRegion(gameStateRegion, false, 0, sizeof(VIZGameState));
        vizGameStateSM = static_cast<VIZGameState *>(gameStateRegion->address);
    }
    catch(bip::interprocess_exception &ex){
        //VIZ_Error(VIZ_FUNC, "Failed to create game state.");
        //printf("bip::interprocess_exception &ex");
        throw;
    }

    vizGameStateSM->LEVEL_0 = 1;
    vizGameStateSM->LEVEL_1 = 1;
    vizGameStateSM->BALL_STROKE_0 = 0;
    vizGameStateSM->BALL_STROKE_1 = 0;

    /*
    BYTE value;
    unsigned int position[2];
    unsigned int size[2];

    unsigned int objectId;
    double objectPosition[9];
    char objectName[VIZ_MAX_NAME_LEN];
    
    string temp = "cat";
    char tab2[1024];
    strncpy(tab2, temp.c_str(), sizeof(tab2));
    tab2[sizeof(tab2) - 1] = 0;
    */

    std::string temp = "ball_1";
    VIZLabel *ball_0 = new VIZLabel();
    strncpy(ball_0->objectName, temp.c_str(), sizeof(ball_0->objectName));
    ball_0->objectName[sizeof(ball_0->objectName) - 1] = 0;
    ball_0->position[0] = 2;
    ball_0->position[1] = 7;
    ball_0->objectId = 0;

    vizGameStateSM->LABEL_COUNT = 1;
    vizGameStateSM->LABEL[0] = *ball_0;

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
    //printf("VIZ_GameStateSMUpdate\n");

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


void VIZ_GameStateUpdateReward(unsigned int reward) {
    //printf("VIZ_GameStateUpdateReward\n");
    int max = 100;
    int min = 1;

    int output = min + (rand() % static_cast<int>(max - min + 1));
    //printf("output: %d\n", output);

    vizGameStateSM->REWARD = reward;
}


void VIZ_GameStateUpdateVariables(int map_killcount) {
    //printf("VIZ_GameStateUpdateVariables\n");
    //printf("map_killcount: %d\n", map_killcount);
    int max = 100;
    int min = 1;

    int output = min + (rand() % static_cast<int>(max - min + 1));
}


void VIZ_InputInit() {
    try {
        //printf("VIZ_InputInit\n");
        VIZSMRegion* inputRegion = &VIZ_SM_INPUTSTATE;
        VIZ_SMCreateRegion(inputRegion, true, VIZ_SMGetRegionOffset(inputRegion), sizeof(VIZInputState));
        vizInput = static_cast<VIZInputState *>(inputRegion->address);
    }
    catch (bip::interprocess_exception &ex) {
        throw;
    }

    for (size_t i = 0; i < VIZ_BT_COUNT; ++i) {
        vizInput->BT[i] = 0;
        vizInput->BT_AVAILABLE[i] = false;
        vizLastInputBT[i] = 0;
        vizLastInputUpdate[i] = 0;
    }

    for(size_t i = 0; i < VIZ_BT_AXIS_BT_COUNT; ++i){
        vizInput->BT_MAX_VALUE[i] = 0;
    }
}


void VIZ_GetInputState() {
    //vizInput->BT[0] = 1;
    //vizInput->BT[1] = 2;
    //vizInput->BT[2] = 3;

    //printf("vizInput->BT[0]: %f\n", vizInput->BT[0]);
    //printf("vizInput->BT[1]: %f\n", vizInput->BT[1]);
    //printf("vizInput->BT[2]: %f\n", vizInput->BT[2]);

    for (size_t i = 0; i < VIZ_BT_COUNT; ++i) {
        bool available_button = vizInput->BT_AVAILABLE[i];
        if (available_button != false) {
            //printf("i: %d\n", i);
            //printf("available_button: %d\n", available_button);
            //printf("vizInput->BT[i]: %f\n", vizInput->BT[i]);
            ;
        }
    }
}


void VIZ_ScreenInit() {
    unsigned int vizScreenWidth = 640;
    unsigned int vizScreenHeight = 480;
    vizScreenChannelSize = sizeof(BYTE) * vizScreenWidth * vizScreenHeight * 4;
    vizScreenSize = vizScreenChannelSize;

    VIZ_SMUpdate(vizScreenSize);

    try {
        //printf("VIZ_ScreenInit\n");
        VIZSMRegion *screenRegion = &vizSMRegion[VIZ_SM_SCREEN_NUM];
        VIZ_SMCreateRegion(screenRegion, false, VIZ_SMGetRegionOffset(screenRegion), vizScreenSize);
        memset(screenRegion->address, 0, screenRegion->size);
    }
    catch(...){ // bip::interprocess_exception
        //printf("bip::interprocess_exception\n");
        throw;
    }

    vizScreenSM = static_cast<BYTE *>(VIZ_SM_SCREEN.address);
}


void VIZ_ScreenBufferUpdate(unsigned char *screen_buffer) {
    unsigned int vizScreenWidth = 640;
    unsigned int vizScreenHeight = 480;
    int screenChannelSize = sizeof(BYTE) * vizScreenWidth * vizScreenHeight * 4;
    int screenSize = screenChannelSize;

    for(unsigned int i = 0; i < screenSize; ++i){
        //printf("i: %d\n", i);
        //printf("screen_buffer[i]: %d\n", screen_buffer[i]);
        vizScreenSM[i] = screen_buffer[i];
    }

    //memcpy(vizScreenSM, screen_buffer, sizeof(screen_buffer));
}