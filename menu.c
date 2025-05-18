#include "menu.h"
#include "game.h"
#include "utils.h"
#include "spell.h"
#include <allegro.h>
#include <stdio.h>
#include "../include/images.h"


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
    draw_button(300, 410, 200, 50, hardcore_mode ? "Mode : Hardcore" : "Mode : Normal");
    if (mouse_b & 1 && mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 410 && mouse_y <= 460) {
        hardcore_mode = !hardcore_mode;
        rest(200);
    }
}
void show_rules() {
    clear_to_color(buffer, makecol(20, 20, 60));
    draw_text_center("Règles détaillées du jeu ECE Arena", SCREEN_W / 2, 30, makecol(255, 255, 0));

    int y = 70;
    draw_text("- Jeu de combat en arène au tour par tour (inspiré de Dofus Arena)", 20, y, makecol(255, 255, 255));
    draw_text("- 2 à 4 joueurs s'affrontent sur une grille 10x10", 20, y += 20, makecol(255, 255, 255));
    draw_text("- Chaque joueur choisit une classe avec 4 sorts différents", 20, y += 20, makecol(255, 255, 255));
    draw_text("- À son tour, un joueur dispose de :", 20, y += 30, makecol(255, 255, 255));
    draw_text("    - 3 PM (Points de Mouvement)", 40, y += 20, makecol(255, 255, 255));
    draw_text("    - 6 PA (Points d'Action)", 40, y += 20, makecol(255, 255, 255));
    draw_text("    - 20 secondes pour jouer (puis passage automatique)", 40, y += 20, makecol(255, 255, 255));
    draw_text("- Le joueur peut : se déplacer, lancer un sort, ou finir son tour", 20, y += 30, makecol(255, 255, 255));
    draw_text("- Les déplacements sont limités aux cases accessibles (3 PM max)", 20, y += 20, makecol(255, 255, 255));
    draw_text("- Les sorts ont :", 20, y += 30, makecol(255, 255, 255));
    draw_text("    - Un coût en PA", 40, y += 20, makecol(255, 255, 255));
    draw_text("    - Une portée minimale et maximale", 40, y += 20, makecol(255, 255, 255));
    draw_text("    - Un effet : dégâts, soin ou zone", 40, y += 20, makecol(255, 255, 255));
    draw_text("    - Un pourcentage d'échec", 40, y += 20, makecol(255, 255, 255));
    draw_text("- Une attaque réussie fait perdre des PV à l’adversaire", 20, y += 30, makecol(255, 255, 255));
    draw_text("- Un joueur est éliminé si ses PV tombent à zéro", 20, y += 20, makecol(255, 255, 255));
    draw_text("- Le dernier joueur en vie remporte la partie", 20, y += 20, makecol(255, 255, 255));
    draw_text("- À la fin, un classement est affiché", 20, y += 20, makecol(255, 255, 255));
    draw_text("- Bonus : certaines classes peuvent se soigner ou frapper en zone", 20, y += 20, makecol(255, 255, 255));

    draw_button(300, 640, 200, 40, "Retour");
    if (mouse_b & 1 && mouse_x >= 300 && mouse_x <= 500 && mouse_y >= 640 && mouse_y <= 680) {
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

    // Réinitialiser grille
    for (int y = 0; y < 10; y++)
        for (int x = 0; x < 10; x++)
            grid[y][x] = 0;

    // Supprimer l'image des anciens obstacles
    if (img_obstacle_layer != NULL) {
        destroy_bitmap(img_obstacle_layer);
        img_obstacle_layer = NULL;
    }

    // Réinitialiser les joueurs
    for (int i = 0; i < 4; i++) {
        selected_characters[i] = -1;
        player_pm_total[i] = 100;
        player_pm_turn[i] = 3;
        player_pa_turn[i] = 6;
        player_pv[i] = 20;
        player_grid_positions[i][0] = -1;
        player_grid_positions[i][1] = -1;
        is_alive[i] = 1;
        death_order[i] = -1;
    }

    death_count = 0;
    game_over = 0;
    death_message[0] = '\0';
    death_message_timer = 0;
}



