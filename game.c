#include "game.h"
#include "movement.h"
#include "utils.h"
#include "menu.h"


extern MovementPath movement_path;

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
    highlight_accessible_cells(cx, cy);

    // Affiche visuellement les cases accessibles
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            if (visited[y][x]) {
                rectfill(buffer, x * 60, y * 60, (x + 1) * 60, (y + 1) * 60, makecol(255, 255, 100));
            }
        }
    }

    draw_countdown(countdown);
    draw_pm_info(current_player);

    // Bouton retour menu
    draw_button(10, 650, 200, 40, "Retour au menu");

    if (mouse_b & 1 && mouse_x >= 10 && mouse_x <= 210 && mouse_y >= 650 && mouse_y <= 690) {
        game_state = STATE_MENU;
        movement_path.active = false;
        return;
    }

    // Bouton fin de tour
    draw_button(600, 650, 180, 40, "Fin de tour");
    static int click_cooldown = 0;

    if (mouse_b & 1 && mouse_x >= 600 && mouse_x <= 780 && mouse_y >= 650 && mouse_y <= 690) {
        if (click_cooldown == 0) {
            next_turn();
            click_cooldown = 10;
        }
    }

    if (click_cooldown > 0) click_cooldown--;

    // Déplacement vers case accessible
    if ((mouse_b & 1) && !movement_path.active && player_pm_turn[current_player] > 0) {
        int gx = mouse_x / 60;
        int gy = mouse_y / 60;

        if (visited[gy][gx] && is_valid_cell(gx, gy)) {
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
        }
    }

    update_movement();

    if (++frame_counter >= 50) {
        countdown--;
        frame_counter = 0;
    }

    if (countdown <= 0) {
        next_turn();
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
    countdown = 20;
    movement_path.active = false;
    movement_path.index = 0;
    movement_path.length = 0;

    int cx = player_grid_positions[current_player][0];
    int cy = player_grid_positions[current_player][1];
    highlight_accessible_cells(cx, cy);
}

