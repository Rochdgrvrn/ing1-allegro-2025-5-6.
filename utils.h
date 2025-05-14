#ifndef UTILS_H
#define UTILS_H

#include <allegro.h>

void draw_button(int x, int y, int w, int h, const char *label);
void draw_text(const char *text, int x, int y, int color);
void draw_text_center(const char *text, int x, int y, int color);
void draw_custom_character(int x, int y, int character);
void draw_countdown(int time_left);
void draw_pm_info(int player);
void draw_grid(void);

#endif

