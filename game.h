#ifndef GAME_H
#define GAME_H
#include &lt;allegro.h&gt;
typedef enum {
STATE_MENU,
STATE_RULES,
STATE_CHARACTER_SELECTION,
STATE_GAME
} GameState;
typedef enum {
KNIGHT,
NINJA,
SAMURAI,
MILITARY,
CHARACTER_COUNT
} Character;
extern int selected_characters[2];
extern int current_player;
extern GameState game_state;
extern BITMAP *buffer;
extern int player_positions[2][2];
extern int move_speed;

extern int player_moves_left[2];
extern int countdown;
extern int frame_counter;
void draw_button(int x, int y, int w, int h, const char *label);
void draw_custom_character(int x, int y, Character character);
void draw_countdown(int time_left);
void show_menu();
void show_rules();
void show_character_selection();
void next_turn();
void handle_movement_buttons(int x, int y, int w, int h, const char* direction);
void show_game();
#endif
