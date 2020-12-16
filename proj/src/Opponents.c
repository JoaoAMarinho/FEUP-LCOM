#include "Opponents.h"

extern uint16_t horizontal_res, vertical_res;

Opponent ** gameOpponents;

//---------------------------------------------------------------------------------------------

void LoadOpponents(){
    gameOpponents = (Opponent **) malloc(5 * sizeof(Opponent *));

    gameOpponents[0] = create_opponent(500,110,UP,CAFETERIA);
    gameOpponents[1] = create_opponent(500,170,DOWN,HALLWAY1);
    gameOpponents[2] = create_opponent(500,230,LEFT,NAVIGATION);
    gameOpponents[3] = create_opponent(500,290,RIGHT,WEAPONS);
    gameOpponents[4] = create_opponent(27,524, UP, ADMIN);

}

Opponent* create_opponent(int x, int y, Direction direction, Room_number currentRoom){
    Opponent* opponent = (Opponent *) malloc (sizeof(Opponent));

    opponent->x = x;
    opponent->y = y;

    opponent->opponentRoom=currentRoom;
    opponent->direction = direction;
    opponent->xspeed = 4;
    opponent->yspeed = 4;
    opponent->alive = true;
    //enemy->moveAnimation = 0;  saber a animação usar um static

    //enemy->isMoving = true;
    //enemy->isAttacking = false;

    xpm_image_t img;

    //Load opponent animations
    xpm_load(opponent_anim0_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[0] = img;
    xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[1] = img;
    xpm_load(opponent_anim2_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[2] = img;
    xpm_load(opponent_anim1_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[3] = img;
    xpm_load(opponent_anim0_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[4] = img;
    xpm_load(opponent_anim3_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[5] = img;
    xpm_load(opponent_anim4_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[6] = img;
    xpm_load(opponent_anim3_xpm, XPM_8_8_8_8, &img);
    opponent->opponentAnimations[7] = img;

    free(&img);
    opponent->opponentImg = opponent->opponentAnimations[0];

    return opponent;
}
