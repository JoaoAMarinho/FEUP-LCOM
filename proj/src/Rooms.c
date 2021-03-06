#include "Rooms.h"

//Include dos xpm dos rooms
#include "Xpms/Rooms/Cafeteria.xpm"
#include "Xpms/Rooms/CafeteriaObstacles.xpm"
#include "Xpms/Rooms/Hallway1.xpm"
#include "Xpms/Rooms/Hallway1Obstacles.xpm"
#include "Xpms/Rooms/Admin.xpm"
#include "Xpms/Rooms/AdminObstacles.xpm"
#include "Xpms/Rooms/Navigation.xpm"
#include "Xpms/Rooms/NavigationObstacles.xpm"
#include "Xpms/Rooms/Weapons.xpm"
#include "Xpms/Rooms/WeaponsObstacles.xpm"
#include "Xpms/Rooms/Eletrical.xpm"
#include "Xpms/Rooms/EletricalObstacles.xpm"
#include "Xpms/Rooms/Hallway2.xpm"
#include "Xpms/Rooms/Hallway2Obstacles.xpm"
#include "Xpms/Rooms/Hallway3.xpm"
#include "Xpms/Rooms/Hallway3Obstacles.xpm"
#include "Xpms/Rooms/Medbay.xpm"
#include "Xpms/Rooms/MedbayObstacles.xpm"
#include "Xpms/Rooms/Reactor.xpm"
#include "Xpms/Rooms/ReactorObstacles.xpm"
#include "Xpms/Rooms/UpperEngine.xpm"
#include "Xpms/Rooms/UpperEngineObstacles.xpm"
#include "Xpms/Rooms/LowerEngine.xpm"
#include "Xpms/Rooms/LowerEngineObstacles.xpm"
#include "Xpms/Rooms/Security.xpm"
#include "Xpms/Rooms/SecurityObstacles.xpm"

extern uint16_t horizontal_res, vertical_res;

Room * room;

//---------------------------------------------------------------------------------------------

Room * load_room(Room_number roomNumber){
    free(room);
    room = (Room *) malloc (sizeof(room));
    room->currentRoom=roomNumber;
    //CAFETERIA, HALLWAY1, ADMIN, WEAPONS, NAVIGATION, HALLWAY2, MEDBAY, ELETRICAL, UPPERENG, HALLWAY3, LOWERENG, REACTOR, SECURITY, END
    if(roomNumber==CAFETERIA){
        xpm_load(Cafeteria_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(CafeteriaObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==HALLWAY1){
        xpm_load(Hallway1_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(Hallway1Obstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==ADMIN){
        xpm_load(Admin_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(AdminObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==WEAPONS){
        xpm_load(Weapons_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(WeaponsObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);

    }
    else if(roomNumber==NAVIGATION){
        xpm_load(Navigation_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(NavigationObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==HALLWAY2){
        xpm_load(Hallway2_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(Hallway2Obstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==MEDBAY){
        xpm_load(Medbay_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(MedbayObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==ELETRICAL){
        xpm_load(Eletrical_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(EletricalObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==UPPERENG){
        xpm_load(UpperEngine_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(UpperEngineObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==HALLWAY3){
        xpm_load(Hallway3_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(Hallway3Obstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==LOWERENG){
        xpm_load(LowerEngine_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(LowerEngineObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==REACTOR){
        xpm_load(Reactor_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(ReactorObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }
    else if(roomNumber==SECURITY){
        xpm_load(Security_xpm, XPM_8_8_8_8, &room->roomBackground);
        xpm_load(SecurityObstacles_xpm, XPM_8_8_8_8, &room->roomObstacles);
    }

    return room;
    
}

void draw_room(){
    uint32_t* map=(uint32_t*)room->roomBackground.bytes;
    
    for(int row = 0;row < room->roomBackground.height; row++){
        for(int column = 0;column < room->roomBackground.width; column++){
            if (*(map + column + row*room->roomBackground.width) != xpm_transparency_color(XPM_8_8_8_8))
                drawPixel(column,row,*(map + column + row*room->roomBackground.width));
        }
    }
}

void delete_room(){
    if (room != NULL){
        free(room->roomBackground.bytes);
        free(room);
        room = NULL; 
    }
}

