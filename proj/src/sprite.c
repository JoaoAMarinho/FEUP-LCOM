
#include "sprite.h"

/** Creates a new sprite with pixmap "pic", with specified
* position (within the screen limits) and speed;
* Does not draw the sprite on the screen
* Returns NULL on invalid pixmap.
*/
static uint16_t* map;
Sprite *create_sprite(xpm_map_t pic, int x, int y,int xspeed, int yspeed, enum xpm_image_type xpm_type) {
    //allocate space for the "object"
    Sprite *sp = (Sprite *) malloc ( sizeof(Sprite));
    xpm_image_t img;

    if( sp == NULL )
        return NULL;
    // read the sprite pixmap
    xpm_load(pic, xpm_type, &img);
    map=(uint16_t*)img.bytes;
    sp->map=(char *)img.bytes;
   
    if( sp->map == NULL ) {
        free(sp);
        return NULL;
    }
    sp->width = img.width; sp->height=img.height;
    sp->x = x; sp->y = y; sp->xspeed = xspeed; sp->yspeed = yspeed;

    return sp;
}

void destroy_sprite(Sprite *sp) {
    if( sp == NULL )
        return;
    if( sp ->map )
        free(sp->map);
    free(sp);
    sp = NULL; // XXX: pointer is passed by value
    // should do this @ the caller
}


int animate_sprite(Sprite *sp) {return 1;}

int draw_sprite(Sprite *sp, char *base) {
    for(int row = 0;row < sp->height; row++){
        for(int column = 0;column < sp->width; column++){

            //drawPixel(sp->x+column,sp->y+row,sp->map[sp->width*row+column]);
            drawPixel(sp->x+column,sp->y+row,*(map+sp->width*row+column));

        }
    }
    return 0;
}

int check_collision(Sprite *sp, char *base) {return 1;}
