#include "../include/utils.h"
#include "../include/game.h"
#include <stdio.h>


extern BITMAP *buffer;
extern int player_pm_total[4];
extern int player_pm_turn[4];
extern int grid[10][10];

void draw_button(int x, int y, int w, int h, const char *label) {
    rectfill(buffer, x, y, x + w, y + h, makecol(100, 100, 100));
    rect(buffer, x, y, x + w, y + h, makecol(255, 255, 255));
    textout_centre_ex(buffer, font, label, x + w / 2, y + h / 3, makecol(255, 255, 255), -1);
}

void draw_text(const char *text, int x, int y, int color) {
    textout_ex(buffer, font, text, x, y, color, -1);
}

void draw_text_center(const char *text, int x, int y, int color) {
    textout_centre_ex(buffer, font, text, x, y, color, -1);
}

void draw_custom_character(int x, int y, int character) {
    switch (character) {
        case 0: rectfill(buffer, x, y, x + 60, y + 60, makecol(0, 0, 255)); break;
        case 1: rectfill(buffer, x, y, x + 60, y + 60, makecol(0, 0, 0)); break;
        case 2: rectfill(buffer, x, y, x + 60, y + 60, makecol(255, 0, 0)); break;
        case 3: rectfill(buffer, x, y, x + 60, y + 60, makecol(0, 255, 0)); break;
    }
}

void draw_countdown(int time_left) {
    char countdown_text[50];
    sprintf(countdown_text, "Temps: %d s", time_left);
    textout_ex(buffer, font, countdown_text, SCREEN_W - 150, 30, makecol(255, 255, 255), -1);
}

void draw_pm_info(int player) {
    char text[64];
    sprintf(text, "PM totaux : %d", player_pm_total[player]);
    textout_ex(buffer, font, text, SCREEN_W - 200, 60, makecol(255, 255, 255), -1);
    sprintf(text, "PM tour : %d", player_pm_turn[player]);
    textout_ex(buffer, font, text, SCREEN_W - 200, 80, makecol(255, 255, 255), -1);
}

void draw_grid() {
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            int px = x * 60;
            int py = y * 60;
            rect(buffer, px, py, px + 60, py + 60, makecol(255, 255, 255));
            if (grid[y][x] == 9) {
                line(buffer, px, py, px + 60, py + 60, makecol(255, 0, 0));
                line(buffer, px + 60, py, px, py + 60, makecol(255, 0, 0));
            }
        }
    }
}

