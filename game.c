#include "game.h"
#include "movement.h"
#include "utils.h"
#include "menu.h"
#include "spell.h"
#include <stdio.h>

void check_deaths(void);
extern MovementPath movement_path;

int selected_spell_index = -1;

void show_game() {
    clear_to_color(buffer, makecol(0, 100, 100));
    draw_text("Partie en cours...", 300, 20, makecol(255, 255, 255));
    draw_grid();

    for (int i = 0; i < total_players; ++i) {
        int px = player_grid_positions[i][0];
        int py = player_grid_positions[i][1];
        draw_custom_character(px * 60, py * 60, selected_characters[i]);
    }

    int cx = player_grid_positions[current_player][0];
    int cy = player_grid_positions[current_player][1];

    // Met à jour les cases accessibles ou ciblables
    memset(visited, 0, sizeof(visited));
    if (selected_spell_index == -1) {
        highlight_accessible_cells(cx, cy);
    } else {
        Spell s = all_classes[selected_characters[current_player]].spells[selected_spell_index];
        for (int y = 0; y < 10; y++) {
            for (int x = 0; x < 10; x++) {
                int d = abs(x - cx) + abs(y - cy);
                if (d >= s.min_range && d <= s.max_range) {
                    visited[y][x] = 1;
                }
            }
        }
    }

    // Affichage visuel des cases ciblables
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (visited[y][x]) {
                rectfill(buffer, x * 60, y * 60, (x + 1) * 60, (y + 1) * 60, makecol(255, 255, 100));
            }
        }
    }

    draw_countdown(countdown);
    draw_pm_info(current_player);

    // Boutons sorts
    Spell *spells = all_classes[selected_characters[current_player]].spells;
    for (int i = 0; i < 4; i++) {
        int x = 50 + i * 120;
        draw_button(x, 580, 100, 50, spells[i].name);
        if (mouse_b & 1 && mouse_x >= x && mouse_x <= x + 100 && mouse_y >= 580 && mouse_y <= 630) {
            selected_spell_index = i;
            rest(200);
        }
    }

    // Bouton "Move"
    draw_button(600, 580, 150, 40, "Move");
    if (mouse_b & 1 && mouse_x >= 600 && mouse_x <= 750 && mouse_y >= 580 && mouse_y <= 620) {
        selected_spell_index = -1;
        rest(200);
    }

    // Infos du sort sélectionné
    if (selected_spell_index >= 0) {
        Spell s = spells[selected_spell_index];
        char line1[64], line2[64], line3[64], line4[64];
        sprintf(line1, "Sort : %s", s.name);
        sprintf(line2, "PA : %d", s.pa_cost);
        sprintf(line3, "Portée : %d-%d | Zone : %s", s.min_range, s.max_range, s.aoe ? "Oui" : "Non");
        sprintf(line4, "Dégâts : %d-%d | Échec : %d%%", s.min_damage, s.max_damage, s.fail_chance_percent);

        int x = 620, y = 200;
        draw_text(line1, x, y, makecol(255, 255, 0));
        draw_text(line2, x, y + 15, makecol(255, 255, 255));
        draw_text(line3, x, y + 30, makecol(255, 255, 255));
        draw_text(line4, x, y + 45, makecol(255, 255, 255));
    }

    // Bouton "Fin de tour"
    static int click_cooldown = 0;
    draw_button(600, 650, 180, 40, "Fin de tour");
    if (mouse_b & 1 && mouse_x >= 600 && mouse_x <= 780 && mouse_y >= 650 && mouse_y <= 690) {
        if (click_cooldown == 0) {
            next_turn();
            selected_spell_index = -1;
            click_cooldown = 10;
        }
    }
    if (click_cooldown > 0) click_cooldown--;

    // Bouton "Retour au menu" — toujours visible
    draw_button(10, 650, 200, 40, "Retour au menu");
    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 650 && mouse_y <= 690) {
        game_state = STATE_MENU;
        movement_path.active = false;
        selected_spell_index = -1;
        return;
    }

    // Clique sur la grille : déplacement ou attaque
    if ((mouse_b & 1) && !movement_path.active) {
        int gx = mouse_x / 60;
        int gy = mouse_y / 60;
        if (gx >= 0 && gx < 10 && gy >= 0 && gy < 10 && visited[gy][gx]) {
            int cx = player_grid_positions[current_player][0];
            int cy = player_grid_positions[current_player][1];

            if (selected_spell_index == -1 && player_pm_turn[current_player] > 0) {
                Coord path[100];
                int len = 0;
                if (find_path(cx, cy, gx, gy, path, &len) && len - 1 <= player_pm_turn[current_player]) {
                    movement_path.active = true;
                    movement_path.index = 1;
                    movement_path.length = len;
                    for (int i = 0; i < len; i++) {
                        movement_path.coords[i] = path[i];
                    }
                }
            } else if (selected_spell_index >= 0) {
                Spell s = spells[selected_spell_index];
                if (player_pa_turn[current_player] >= s.pa_cost) {
                    int target_id = grid[gy][gx] - 1;
                    if (target_id >= 0 && target_id < total_players) {
                        if ((rand() % 100) >= s.fail_chance_percent) {
                            int value = s.min_damage + rand() % (s.max_damage - s.min_damage + 1);
                            if (s.heal) {
                                player_pv[target_id] += value;
                                if (player_pv[target_id] > 20) player_pv[target_id] = 20;
                            } else {
                                player_pv[target_id] -= value;
                                if (player_pv[target_id] < 0) player_pv[target_id] = 0;
                            }
                        }
                        player_pa_turn[current_player] -= s.pa_cost;
                        selected_spell_index = -1;
                    }
                }
            }
        }
    }

    update_movement();
    check_deaths();


    if (++frame_counter >= 50) {
        countdown--;
        frame_counter = 0;
    }

    if (countdown <= 0) {
        next_turn();
        selected_spell_index = -1;
    }
    if (game_over) {
        clear_to_color(buffer, makecol(0, 0, 0));
        draw_text_center("Fin de partie !", SCREEN_W / 2, 100, makecol(255, 255, 0));

        for (int i = 0; i < death_count; i++) {
            char line[64];
            sprintf(line, "%d. Joueur %d (%s)", i + 1, death_order[i] + 1,
                    all_classes[selected_characters[death_order[i]]].name);
            draw_text_center(line, SCREEN_W / 2, 150 + i * 20, makecol(255, 255, 255));
        }

        draw_button(100, 600, 200, 40, "Rejouer");
        draw_button(310, 600, 200, 40, "Nouvelle partie");
        draw_button(520, 600, 200, 40, "Quitter");

        if (mouse_b & 1 && mouse_x >= 100 && mouse_x <= 300 && mouse_y >= 600 && mouse_y <= 640) {
            reset_game_state();       // tu dois l’avoir dans menu.c
            place_players_and_obstacles();
            game_over = 0;
            death_count = 0;
            for (int i = 0; i < 4; i++) {
                is_alive[i] = 1;
                death_order[i] = -1;
            }
            game_state = STATE_GAME;
        }

        if (mouse_b & 1 && mouse_x >= 310 && mouse_x <= 510 && mouse_y >= 600 && mouse_y <= 640) {
            reset_game_state();
            game_state = STATE_MENU;
            game_over = 0;
            death_count = 0;
            for (int i = 0; i < 4; i++) {
                is_alive[i] = 1;
                death_order[i] = -1;
            }
        }

        if (mouse_b & 1 && mouse_x >= 520 && mouse_x <= 720 && mouse_y >= 600 && mouse_y <= 640) {
            exit(0);
        }

        return;  // NE PAS CONTINUER LE JEU
    }
}

void place_players_and_obstacles() {
    srand(time(NULL));
    int placed_players = 0, placed_obstacles = 0;

    while (placed_players < total_players) {
        int x = rand() % 10;
        int y = rand() % 10;
        if (grid[y][x] == 0) {
            grid[y][x] = placed_players + 1;
            player_grid_positions[placed_players][0] = x;
            player_grid_positions[placed_players][1] = y;
            placed_players++;
        }
    }

    while (placed_obstacles < 2) {
        int x = rand() % 10;
        int y = rand() % 10;
        if (grid[y][x] == 0) {
            grid[y][x] = 9;
            placed_obstacles++;
        }
    }
}

void next_turn() {
    current_player = (current_player + 1) % total_players;
    player_pm_turn[current_player] = 3;
    player_pa_turn[current_player] = 6;
    countdown = 20;
    movement_path.active = false;
    movement_path.index = 0;
    movement_path.length = 0;

    int cx = player_grid_positions[current_player][0];
    int cy = player_grid_positions[current_player][1];
    highlight_accessible_cells(cx, cy);
}

void check_deaths() {
    for (int i = 0; i < total_players; i++) {
        if (is_alive[i] && player_pv[i] <= 0) {
            is_alive[i] = 0;
            death_order[death_count++] = i;

            // Retire le joueur de la grille
            int x = player_grid_positions[i][0];
            int y = player_grid_positions[i][1];
            grid[y][x] = 0;
        }
    }

    // Si un seul joueur vivant → fin de partie
    int alive = 0, last = -1;
    for (int i = 0; i < total_players; i++) {
        if (is_alive[i]) {
            alive++;
            last = i;
        }
    }

    if (alive == 1 && game_over == 0) {
        death_order[death_count++] = last;
        game_over = 1;
    }
}

