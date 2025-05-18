#include <allegro.h>
#include "../include/game.h"
#include "../include/images.h"

BITMAP *buffer = NULL;
BITMAP *img_obstacle_layer = NULL;  // ✅ CE QU’IL MANQUAIT !
BITMAP *img_attack_flash = NULL;
BITMAP *img_fireball = NULL;
BITMAP *img_explosion = NULL;
BITMAP *img_heal_wave = NULL;


GameState game_state = STATE_MENU;

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
int player_pv[4] = {20, 20, 20, 20};
int player_pa_turn[4] = {6, 6, 6, 6};
int is_alive[4] = {1, 1, 1, 1};
int death_order[4] = {-1, -1, -1, -1};
int death_count = 0;
int game_over = 0;
char death_message[64] = "";
int death_message_timer = 0;
int hardcore_mode = 0;
int oneshot_timer = 0;



