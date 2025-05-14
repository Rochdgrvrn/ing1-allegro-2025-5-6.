#ifndef GAME_H
#define GAME_H

#include <allegro.h>

typedef enum {
    STATE_MENU,
    STATE_RULES,
    STATE_PLAYER_COUNT_SELECTION,
    STATE_CHARACTER_SELECTION,
    STATE_GAME
} GameState;

extern GameState game_state;
extern BITMAP *buffer;

extern int current_player;
extern int total_players;
extern int selected_characters[4];
extern int player_grid_positions[4][2];
extern int player_pm_total[4];
extern int player_pm_turn[4];
extern int countdown;
extern int frame_counter;
extern int grid[10][10];
extern int visited[10][10];
void next_turn(void);
void show_game(void);
void place_players_and_obstacles(void);




#endif
