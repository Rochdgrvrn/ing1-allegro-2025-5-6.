#include "game.h"
#include "movement.h"
#include "utils.h"
#include "menu.h"
#include "spell.h"
#include <stdio.h>
#include "../include/images.h"
#include "../include/assets.h"



extern MovementPath movement_path;
extern int oneshot_timer;
int selected_spell_index = -1;

void animate_attack(int gx, int gy, int type);  // déclaration du prototype

int get_animation_type(Spell s) {
    if (s.heal) return 3;
    if (s.aoe) return 2;
    if (s.max_range > 1) return 1;
    return 0;
}



void check_deaths(void) {
    for (int i = 0; i < total_players; i++) {
        if (is_alive[i] && player_pv[i] <= 0) {
            is_alive[i] = 0;
            death_order[death_count++] = i;

            int x = player_grid_positions[i][0];
            int y = player_grid_positions[i][1];
            grid[y][x] = 0;

            sprintf(death_message, "💀 Joueur %d (%s) est mort !", i + 1,
                    all_classes[selected_characters[i]].name);
            death_message_timer = 150;
        }
    }

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

void show_game() {
    clear_to_color(buffer, makecol(0, 100, 100));
    draw_text("Partie en cours...", 300, 20, makecol(255, 255, 255));
    draw_grid();

    for (int i = 0; i < total_players; ++i) {
        if (!is_alive[i]) continue;
        int px = player_grid_positions[i][0];
        int py = player_grid_positions[i][1];
        draw_custom_character(px * 60, py * 60, selected_characters[i]);
    }

    int cx = player_grid_positions[current_player][0];
    int cy = player_grid_positions[current_player][1];

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

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (visited[y][x]) {
                rectfill(buffer, x * 60, y * 60, (x + 1) * 60, (y + 1) * 60, makecol(255, 255, 100));
            }
        }
    }

    draw_countdown(countdown);
    draw_pm_info(current_player);

    Spell *spells = all_classes[selected_characters[current_player]].spells;
    for (int i = 0; i < 4; i++) {
        if (hardcore_mode && i != 0) continue;  // Autorise seulement le sort 0 (CAC) en hardcore
        int x = 50 + i * 120;
        draw_button(x, 580, 100, 50, spells[i].name);
        if (mouse_b & 1 && mouse_x >= x && mouse_x <= x + 100 && mouse_y >= 580 && mouse_y <= 630) {
            selected_spell_index = i;
            rest(200);
        }
    }


    draw_button(600, 580, 150, 40, "Move");
    if (mouse_b & 1 && mouse_x >= 600 && mouse_x <= 750 && mouse_y >= 580 && mouse_y <= 620) {
        selected_spell_index = -1;
        rest(200);
    }

    if (selected_spell_index >= 0) {
        Spell s = spells[selected_spell_index];
        int x = 620, y = 200;

        draw_text("Sort :", x, y, makecol(255, 255, 0));
        draw_text(s.name, x + 50, y, makecol(255, 255, 255));

        draw_text("PA :", x, y + 15, makecol(255, 255, 0));
        char pa_text[16]; sprintf(pa_text, "%d", s.pa_cost);
        draw_text(pa_text, x + 50, y + 15, makecol(255, 255, 255));

        draw_text("Portée :", x, y + 30, makecol(255, 255, 0));
        char range_text[32]; sprintf(range_text, "%d - %d", s.min_range, s.max_range);
        draw_text(range_text, x + 70, y + 30, makecol(255, 255, 255));

        draw_text("Zone :", x, y + 45, makecol(255, 255, 0));
        draw_text(s.aoe ? "Oui" : "Non", x + 70, y + 45, makecol(255, 255, 255));

        draw_text(s.heal ? "Soin :" : "Dégâts :", x, y + 60, makecol(255, 255, 0));
        char dmg_text[32]; sprintf(dmg_text, "%d - %d", s.min_damage, s.max_damage);
        draw_text(dmg_text, x + 70, y + 60, makecol(255, 255, 255));

        draw_text("Échec :", x, y + 75, makecol(255, 255, 0));
        char fail_text[16]; sprintf(fail_text, "%d%%", s.fail_chance_percent);
        draw_text(fail_text, x + 70, y + 75, makecol(255, 255, 255));
    }


    draw_button(600, 650, 180, 40, "Fin de tour");
    static int click_cooldown = 0;
    if (mouse_b & 1 && mouse_x >= 600 && mouse_x <= 780 && mouse_y >= 650 && mouse_y <= 690) {
        if (click_cooldown == 0) {
            next_turn();
            selected_spell_index = -1;
            click_cooldown = 10;
        }
    }
    if (click_cooldown > 0) click_cooldown--;

    draw_button(10, 650, 200, 40, "Retour au menu");
    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 650 && mouse_y <= 690) {
        reset_game_state();
        game_state = STATE_MENU;
        movement_path.active = false;
        selected_spell_index = -1;
        return;
    }

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
                    if (target_id >= 0 && target_id < total_players && is_alive[target_id]) {
                        if ((rand() % 100) >= s.fail_chance_percent) {
                            int anim_type = get_animation_type(s);
                            animate_attack(gx, gy, anim_type);

                            if (strcmp(s.name, "Hurlement") == 0) {
                                player_pa_turn[current_player] += 1;
                                if (player_pa_turn[current_player] > 10) player_pa_turn[current_player] = 10;
                            } else if (strcmp(s.name, "Esquive") == 0) {
                                player_pm_turn[current_player] += 1;
                                if (player_pm_turn[current_player] > 5) player_pm_turn[current_player] = 5;
                            } else {
                                if (s.heal) {
                                    int value = s.min_damage + rand() % (s.max_damage - s.min_damage + 1);
                                    player_pv[target_id] += value;
                                    if (player_pv[target_id] > 20) player_pv[target_id] = 20;
                                } else {
                                    if (hardcore_mode) {
                                        player_pv[target_id] = 0; // 💀 One shot
                                        oneshot_timer = 50;
                                    } else {
                                        int value = s.min_damage + rand() % (s.max_damage - s.min_damage + 1);
                                        player_pv[target_id] -= value;
                                        if (player_pv[target_id] < 0) player_pv[target_id] = 0;
                                    }
                                }

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

    if (death_message_timer > 0) {
        int box_w = 500;
        int box_h = 60;
        int x = (SCREEN_W - box_w) / 2;
        int y = 40;

        rectfill(buffer, x, y, x + box_w, y + box_h, makecol(0, 0, 0));
        rect(buffer, x, y, x + box_w, y + box_h, makecol(255, 0, 0));
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, y + 20, makecol(255, 50, 50), -1, "%s", death_message);

        death_message_timer--;
    }

    if (oneshot_timer > 0) {
        int box_w = 300;
        int box_h = 50;
        int x = (SCREEN_W - box_w) / 2;
        int y = SCREEN_H / 2 - 100;

        rectfill(buffer, x, y, x + box_w, y + box_h, makecol(0, 0, 0));
        rect(buffer, x, y, x + box_w, y + box_h, makecol(255, 0, 0));
        textprintf_centre_ex(buffer, font, SCREEN_W / 2, y + 15, makecol(255, 0, 0), -1, "💥 One Shot !");
        oneshot_timer--;
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
            reset_game_values(); // ✅ garde les personnages
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

        return;
    }
}

void place_players_and_obstacles() {
    srand(time(NULL));

    // 1. Réinitialiser entièrement la grille
    for (int y = 0; y < 10; y++)
        for (int x = 0; x < 10; x++)
            grid[y][x] = 0;

    // 2. Supprimer l’ancienne couche visuelle
    if (img_obstacle_layer != NULL) {
        destroy_bitmap(img_obstacle_layer);
        img_obstacle_layer = NULL;
    }

    // 3. Placer exactement 2 obstacles sur des cases vides
    int placed_obstacles = 0;
    while (placed_obstacles < 2) {
        int x = rand() % 10;
        int y = rand() % 10;
        if (grid[y][x] == 0) {
            grid[y][x] = 9;
            placed_obstacles++;
        }
    }

    // 4. Placer les joueurs ensuite (cases libres ≠ 9)
    int placed_players = 0;
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

    // 5. Générer la couche visuelle d’obstacles
    img_obstacle_layer = create_bitmap(600, 600);
    clear_to_color(img_obstacle_layer, makecol(255, 0, 255));

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (grid[y][x] == 9 && img_obstacle != NULL) {
                masked_blit(img_obstacle, img_obstacle_layer, 0, 0, x * 60, y * 60, 60, 60);
            }
        }
    }
}

void next_turn() {
    int start = current_player;

    do {
        current_player = (current_player + 1) % total_players;
    } while (!is_alive[current_player] && current_player != start);

    player_pm_turn[current_player] = hardcore_mode ? 5 : 3;
    player_pa_turn[current_player] = 6;
    countdown = 20;
    movement_path.active = false;
    movement_path.index = 0;
    movement_path.length = 0;

    int cx = player_grid_positions[current_player][0];
    int cy = player_grid_positions[current_player][1];
    highlight_accessible_cells(cx, cy);
}


void animate_attack(int gx, int gy, int type) {
    BITMAP *anim = NULL;
    switch (type) {
        case 0: anim = img_attack_flash; break;
        case 1: anim = img_fireball; break;
        case 2: anim = img_explosion; break;
        case 3: anim = img_heal_wave; break;
        default: return;
    }

    if (!anim) return;

    int px = gx * 60;
    int py = gy * 60;

    for (int i = 0; i < 4; i++) {
        draw_grid(); // pour redessiner l’arène
        draw_sprite(buffer, anim, px, py); // applique la transparence magenta
        blit(buffer, screen, 0, 0, 0, 0, 800, 700);
        rest(50);
    }
}


void reset_game_values() {
    current_player = 0;
    countdown = 20;
    frame_counter = 0;

    for (int i = 0; i < total_players; i++) {
        player_pm_total[i] = 100;
        player_pm_turn[i] = 3;
        player_pa_turn[i] = 6;
        player_pv[i] = 20;
        is_alive[i] = 1;
        death_order[i] = -1;
    }

    death_count = 0;
    game_over = 0;
    death_message[0] = '\0';
    death_message_timer = 0;
}

