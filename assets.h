#ifndef ASSETS_H
#define ASSETS_H

#include <allegro.h>

extern BITMAP *img_arena;
extern BITMAP *img_characters[4];
extern BITMAP *img_obstacle;
extern BITMAP *img_attack_flash;
extern BITMAP *img_fireball;
extern BITMAP *img_explosion;
extern BITMAP *img_heal_wave;


void load_assets(void);
void destroy_assets(void);

#endif
