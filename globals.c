#include <allegro.h>
#include "../include/game.h"

GameState game_state = STATE_MENU;
BITMAP *buffer = NULL;

int current_player = 0;
int total_players = 2;
int selected_characters[4] = {-1, -1, -1, -1};
int player_grid_positions[4][2];
int player_pm_total[4] = {100, 100, 100, 100};
int player_pm_turn[4] = {3, 3, 3, 3};
int countdown = 20;
int frame_counter = 0;
int grid[10][10] = {0};
int visited[10][10] = {0};


