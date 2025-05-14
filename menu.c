#include <allegro.h>
#include <stdio.h>
#include <string.h>
#include "menu.h"
#include "game.h"
#include "utils.h"
#include "game.h"


void reset_game_state() {
    current_player = 0;
    total_players = 2;


    for (int i = 0; i < 4; i++) {
        selected_characters[i] = -1;
        player_pm_total[i] = 100;
        player_pm_turn[i] = 3;
        player_grid_positions[i][0] = -1;
        player_grid_positions[i][1] = -1;
    }

    memset(grid, 0, sizeof(grid));
    memset(visited, 0, sizeof(visited));
}

void show_menu() {
    reset_game_state();  // 🔁 Réinitialisation à chaque retour menu

    clear_to_color(buffer, makecol(0, 0, 0));
    draw_button(300, 200, 200, 50, "Commencer une partie");
    draw_button(300, 300, 200, 50, "Afficher les règles");

    if (mouse_b & 1) {
        if (mouse_x >= 300 && mouse_x <= 500) {
            if (mouse_y >= 200 && mouse_y <= 250) {
                game_state = STATE_PLAYER_COUNT_SELECTION;
                rest(200);
            } else if (mouse_y >= 300 && mouse_y <= 350) {
                game_state = STATE_RULES;
                rest(200);
            }
        }
    }
}

void show_rules() {
    clear_to_color(buffer, makecol(50, 50, 50));
    draw_text("Règles :", 50, 50, makecol(255, 255, 255));
    draw_text("- Cliquez sur une case en surbrillance pour vous déplacer.", 50, 80, makecol(255, 255, 255));
    draw_button(10, 550, 200, 40, "Retour au menu");

    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 550 && mouse_y <= 590) {
        game_state = STATE_MENU;
        rest(200);
    }
}

void show_player_count_selection() {
    clear_to_color(buffer, makecol(0, 0, 0));
    draw_text_center("Choisissez le nombre de joueurs", SCREEN_W / 2, 100, makecol(255, 255, 255));
    draw_button(250, 200, 100, 50, "2 joueurs");
    draw_button(350, 200, 100, 50, "3 joueurs");
    draw_button(450, 200, 100, 50, "4 joueurs");

    if (mouse_b & 1) {
        if (mouse_y >= 200 && mouse_y <= 250) {
            if (mouse_x >= 250 && mouse_x <= 350) total_players = 2;
            else if (mouse_x >= 350 && mouse_x <= 450) total_players = 3;
            else if (mouse_x >= 450 && mouse_x <= 550) total_players = 4;
            else return;

            for (int i = 0; i < 4; i++) selected_characters[i] = -1;
            current_player = 0;
            game_state = STATE_CHARACTER_SELECTION;
            rest(200);
        }
    }
}

void show_character_selection() {
    clear_to_color(buffer, makecol(30, 30, 30));
    char buf[100];
    sprintf(buf, "Joueur %d: Choisissez un personnage", current_player + 1);
    draw_text_center(buf, SCREEN_W / 2, 50, makecol(255, 255, 255));

    int start_x = 100;
    for (int i = 0; i < 4; ++i) {
        draw_custom_character(start_x + i * 100, 150, i);
        rect(buffer, start_x + i * 100, 150, start_x + i * 100 + 60, 200, makecol(255, 255, 255));
    }

    draw_button(10, 550, 200, 40, "Retour au menu");

    if (mouse_b & 1) {
        if (mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 550 && mouse_y <= 590) {
            game_state = STATE_MENU;
            rest(200);
            return;
        }

        for (int i = 0; i < 4; ++i) {
            int x = start_x + i * 100;
            if (mouse_x >= x && mouse_x <= x + 60 && mouse_y >= 150 && mouse_y <= 200) {
                selected_characters[current_player] = i;
                current_player++;
                rest(200);
                if (current_player >= total_players) {
                    game_state = STATE_GAME;
                    current_player = 0;
                    countdown = 20;
                    place_players_and_obstacles();
                }
                break;
            }
        }
    }
}


