#include "Menus.h"

extern uint16_t horizontal_res, vertical_res;
extern uint8_t keyboard_data;
extern struct packet mouse_pack;

extern Menu gameMenu;
extern Player * player;
extern Room * room;
extern Cursor * cursor;
//extern Date * date;

//xpm_image_t background_menu;
//xpm_image_t clean_background_menu;

//static Button ** pauseMenuButtons;
//static Button ** mainMenuButtons;
//static Button * continueButton;

//---------------------------------------------------------------------------------------------

void Main_ih(Device device){return;}

void Bestscores_ih(Device device){return;}

void Instructions_ih(Device device){return;}

void Pause_ih(Device device){return;}

void Victory_ih(Device device){return;}

void Defeat_ih(Device device){return;}
