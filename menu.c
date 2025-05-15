#include "menu.h"
#include "game.h"
#include "utils.h"
#include "spell.h"
#include <allegro.h>
#include <stdio.h>

int selected_temp = -1;

void show_menu() {
    clear_to_color(buffer, makecol(30, 30, 30));
    draw_text_center("ECE Arena", SCREEN_W / 2, 100, makecol(255, 255, 0));

    draw_button(300, 200, 200, 50, "Jouer");
    draw_button(300, 270, 200, 50, "Règles");
    draw_button(300, 340, 200, 50, "Quitter");

    if (mouse_b & 1) {
        if (mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 200 && mouse_y <= 250) {
            game_state = STATE_PLAYER_COUNT_SELECTION;
            rest(200);
        }
        if (mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 270 && mouse_y <= 320) {
            game_state = STATE_RULES;
            rest(200);
        }
        if (mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 340 && mouse_y <= 390) {
            exit(0);
        }
    }
}

void show_rules() {
    clear_to_color(buffer, makecol(20, 20, 60));
    draw_text_center("Règles du jeu", SCREEN_W / 2, 50, makecol(255, 255, 255));
    draw_text("- Jeu tour par tour avec PA et PM", 50, 100, makecol(255, 255, 255));
    draw_text("- Chaque joueur choisit une classe parmi 4", 50, 130, makecol(255, 255, 255));
    draw_text("- 4 sorts par classe, chacun avec ses stats", 50, 160, makecol(255, 255, 255));
    draw_text("- Déplacez-vous avec Move, attaquez avec vos sorts", 50, 190, makecol(255, 255, 255));
    draw_text("- Objectif : survivre, éliminer les autres", 50, 220, makecol(255, 255, 255));

    draw_button(300, 600, 200, 40, "Retour");
    if (mouse_b & 1 && mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 600 && mouse_y <= 640) {
        game_state = STATE_MENU;
        rest(200);
    }
}

void show_player_count_selection() {
    clear_to_color(buffer, makecol(30, 30, 60));
    draw_text_center("Nombre de joueurs", SCREEN_W / 2, 100, makecol(255, 255, 255));

    for (int i = 2; i <= 4; i++) {
        char label[16];
        sprintf(label, "%d joueurs", i);
        draw_button(300, 150 + (i - 2) * 70, 200, 50, label);
        if (mouse_b & 1 && mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 150 + (i - 2) * 70 && mouse_y <= 200 + (i - 2) * 70) {
            total_players = i;
            current_player = 0;
            game_state = STATE_CHARACTER_SELECTION;
            rest(200);
        }
    }

    draw_button(10, 650, 200, 40, "Retour au menu");
    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 650 && mouse_y <= 690) {
        game_state = STATE_MENU;
        rest(200);
    }
}

void show_character_selection() {
    clear_to_color(buffer, makecol(50, 50, 100));
    char msg[64];
    sprintf(msg, "Joueur %d : choisissez votre personnage", current_player + 1);
    draw_text_center(msg, SCREEN_W / 2, 20, makecol(255, 255, 255));

    int start_x = 100;
    for (int i = 0; i < 4; i++) {
        int x = start_x + i * 100;
        rect(buffer, x, 100, x + 60, 160, makecol(255, 255, 255));
        draw_custom_character(x, 100, i);
        draw_text_center(all_classes[i].name, x + 30, 170, makecol(255, 255, 255));

        if (mouse_b & 1 && mouse_x >= x && mouse_x <= x + 60 && mouse_y >= 100 && mouse_y <= 160) {
            selected_temp = i;
            rest(200);
        }
    }

    if (selected_temp != -1) {
        int y = 220;
        draw_text("Classe sélectionnée :", 100, y, makecol(255, 255, 0));
        draw_text(all_classes[selected_temp].name, 280, y, makecol(255, 255, 255));

        for (int i = 0; i < 4; i++) {
            Spell s = all_classes[selected_temp].spells[i];
            char line[128];
            sprintf(line, "- %s | PA:%d | Portée:%d-%d | Dégâts:%d-%d | Échec:%d%% | Zone:%s",
                    s.name, s.pa_cost, s.min_range, s.max_range,
                    s.min_damage, s.max_damage, s.fail_chance_percent, s.aoe ? "Oui" : "Non");
            draw_text(line, 100, y + 30 + i * 20, makecol(255, 255, 255));
        }

        draw_button(300, 500, 200, 40, "Valider");
        if (mouse_b & 1 && mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 500 && mouse_y <= 540) {
            selected_characters[current_player] = selected_temp;
            selected_temp = -1;
            current_player++;

            if (current_player >= total_players) {
                current_player = 0;
                place_players_and_obstacles();
                game_state = STATE_GAME;
            }
            rest(200);
        }
    }

    draw_button(10, 650, 200, 40, "Retour au menu");
    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 650 && mouse_y <= 690) {
        game_state = STATE_MENU;
        selected_temp = -1;
        rest(200);
    }
}

void reset_game_state() {
    current_player = 0;
    countdown = 20;
    frame_counter = 0;

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            grid[i][j] = 0;

    for (int i = 0; i < 4; i++) {
        selected_characters[i] = -1;
        player_pm_total[i] = 100;
        player_pm_turn[i] = 3;
        player_pa_turn[i] = 6;
        player_pv[i] = 20;
        player_grid_positions[i][0] = -1;
        player_grid_positions[i][1] = -1;
    }
}




