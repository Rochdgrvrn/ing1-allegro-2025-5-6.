#include "../include/utils.h"
#include "../include/game.h"
#include "../include/assets.h"
#include <stdio.h>
#include "../include/images.h"

extern BITMAP *buffer;
extern int player_pm_total[4];
extern int player_pm_turn[4];
extern int player_pa_turn[4];
extern int player_pv[4];
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
    if (character >= 0 && character < 4 && img_characters[character] != NULL) {
        draw_sprite(buffer, img_characters[character], x, y);
    } else {
        // Affiche un carré rouge si image manquante
        rectfill(buffer, x, y, x + 60, y + 60, makecol(255, 0, 0));
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
    sprintf(text, "PA tour : %d", player_pa_turn[player]);
    textout_ex(buffer, font, text, SCREEN_W - 200, 100, makecol(255, 255, 255), -1);
    sprintf(text, "PV : %d", player_pv[player]);
    textout_ex(buffer, font, text, SCREEN_W - 200, 120, makecol(255, 255, 255), -1);
}

void draw_grid() {
    // Affiche le fond de l'arène
    if (img_arena != NULL) {
        draw_sprite(buffer, img_arena, 0, 0);
    } else {
        clear_to_color(buffer, makecol(0, 0, 255));
    }

    // Parcours des cases
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            int px = x * 60;
            int py = y * 60;

            // Affiche l’obstacle si case == 9
            if (grid[y][x] == 9 && img_obstacle != NULL) {
                draw_sprite(buffer, img_obstacle, px, py);
            }

            // Affiche les lignes blanches
            rect(buffer, px, py, px + 60, py + 60, makecol(255, 255, 255));
        }
    }
}

