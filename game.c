#include &quot;game.h&quot;
#include &lt;stdio.h&gt;
#include &lt;string.h&gt;
int selected_characters[2] = {-1, -1};
int current_player = 0;
GameState game_state = STATE_MENU;
BITMAP *buffer;
int player_positions[2][2] = {{100, 300}, {600, 300}};
int move_speed = 67;
int player_moves_left[2] = {3, 3};
int countdown = 20;
int frame_counter = 0;
void draw_button(int x, int y, int w, int h, const char *label) {
rectfill(buffer, x, y, x + w, y + h, makecol(100, 100, 100));
rect(buffer, x, y, x + w, y + h, makecol(255, 255, 255));
textout_centre_ex(buffer, font, label, x + w / 2, y + h / 3, makecol(255, 255, 255), -
1);
}
void draw_custom_character(int x, int y, Character character) {
switch (character) {
case KNIGHT:
rectfill(buffer, x, y, x + 50, y + 50, makecol(0, 0, 255)); break;
case NINJA:
rectfill(buffer, x, y, x + 50, y + 50, makecol(0, 0, 0)); break;
case SAMURAI:
rectfill(buffer, x, y, x + 50, y + 50, makecol(255, 0, 0)); break;
case MILITARY:
rectfill(buffer, x, y, x + 50, y + 50, makecol(0, 255, 0)); break;
}
}
void draw_countdown(int time_left) {
char countdown_text[50];
sprintf(countdown_text, &quot;Temps: %d s&quot;, time_left);

textout_ex(buffer, font, countdown_text, SCREEN_W - 150, 30, makecol(255, 255,
255), -1);
}
void show_menu() {
clear_to_color(buffer, makecol(0, 0, 0));
draw_button(300, 200, 200, 50, &quot;Commencer une partie&quot;);
draw_button(300, 300, 200, 50, &quot;Afficher les règles&quot;);
if (mouse_b &amp; 1) {
if (mouse_x &gt;= 300 &amp;&amp; mouse_x &lt;= 500) {
if (mouse_y &gt;= 200 &amp;&amp; mouse_y &lt;= 250) {
game_state = STATE_CHARACTER_SELECTION;
current_player = 0;
selected_characters[0] = -1;
selected_characters[1] = -1;
rest(200);
} else if (mouse_y &gt;= 300 &amp;&amp; mouse_y &lt;= 350) {
game_state = STATE_RULES;
rest(200);
}
}
}
}
void show_rules() {
clear_to_color(buffer, makecol(50, 50, 50));
textout_ex(buffer, font, &quot;Regles :&quot;, 50, 50, makecol(255, 255, 255), -1);
textout_ex(buffer, font, &quot;- Cliquez pour vous déplacer.&quot;, 50, 80, makecol(255, 255,
255), -1);
textout_ex(buffer, font, &quot;- 3 déplacements ou 20 sec/tour.&quot;, 50, 100, makecol(255,
255, 255), -1);
draw_button(10, 550, 200, 40, &quot;Retour au menu&quot;);
if (mouse_b &amp; 1 &amp;&amp; mouse_x &gt;= 10 &amp;&amp; mouse_x &lt;= 210 &amp;&amp; mouse_y &gt;= 550 &amp;&amp;
mouse_y &lt;= 590) {
game_state = STATE_MENU;
rest(200);
}
}
void show_character_selection() {
clear_to_color(buffer, makecol(30, 30, 30));

char buf[100];
sprintf(buf, &quot;Joueur %d: Choisissez un personnage&quot;, current_player + 1);
textout_centre_ex(buffer, font, buf, SCREEN_W / 2, 50, makecol(255, 255, 255), -
1);
int start_x = 100;
for (int i = 0; i &lt; CHARACTER_COUNT; ++i) {
draw_custom_character(start_x + i * 100, 150, i);
rect(buffer, start_x + i * 100, 150, start_x + i * 100 + 50, 200, makecol(255, 255,
255));
}
draw_button(10, 550, 200, 40, &quot;Retour au menu&quot;);
if (mouse_b &amp; 1) {
if (mouse_x &gt;= 10 &amp;&amp; mouse_x &lt;= 210 &amp;&amp; mouse_y &gt;= 550 &amp;&amp; mouse_y &lt;=
590) {
game_state = STATE_MENU;
rest(200);
return;
}
for (int i = 0; i &lt; CHARACTER_COUNT; ++i) {
int x = start_x + i * 100;
if (mouse_x &gt;= x &amp;&amp; mouse_x &lt;= x + 50 &amp;&amp; mouse_y &gt;= 150 &amp;&amp; mouse_y
&lt;= 200) {
selected_characters[current_player] = i;
current_player++;
rest(200);
if (current_player &gt;= 2) {
game_state = STATE_GAME;
current_player = 0;
countdown = 20;
player_moves_left[0] = 3;
player_moves_left[1] = 3;
}
break;
}
}
}
}
void next_turn() {

current_player = (current_player + 1) % 2;
player_moves_left[current_player] = 3;
countdown = 20;
}
void handle_movement_buttons(int x, int y, int w, int h, const char* direction) {
if (mouse_b &amp; 1 &amp;&amp; mouse_x &gt;= x &amp;&amp; mouse_x &lt;= x + w &amp;&amp; mouse_y &gt;= y &amp;&amp;
mouse_y &lt;= y + h) {
int *pos = player_positions[current_player];
if (player_moves_left[current_player] &gt; 0) {
if (strcmp(direction, &quot;Haut&quot;) == 0) pos[1] -= move_speed;
else if (strcmp(direction, &quot;Bas&quot;) == 0) pos[1] += move_speed;
else if (strcmp(direction, &quot;Gauche&quot;) == 0) pos[0] -= move_speed;
else if (strcmp(direction, &quot;Droite&quot;) == 0) pos[0] += move_speed;
player_moves_left[current_player]--;
rest(200);
}
}
}
void show_game() {
clear_to_color(buffer, makecol(0, 100, 100));
textout_ex(buffer, font, &quot;Partie en cours...&quot;, 300, 20, makecol(255, 255, 255), -1);
draw_custom_character(player_positions[0][0], player_positions[0][1],
selected_characters[0]);
draw_custom_character(player_positions[1][0], player_positions[1][1],
selected_characters[1]);
draw_countdown(countdown);
draw_button(300, 500, 100, 40, &quot;Haut&quot;);
draw_button(300, 600, 100, 40, &quot;Bas&quot;);
draw_button(200, 550, 100, 40, &quot;Gauche&quot;);
draw_button(400, 550, 100, 40, &quot;Droite&quot;);
draw_button(10, 650, 200, 40, &quot;Retour au menu&quot;);
handle_movement_buttons(300, 500, 100, 40, &quot;Haut&quot;);
handle_movement_buttons(300, 600, 100, 40, &quot;Bas&quot;);
handle_movement_buttons(200, 550, 100, 40, &quot;Gauche&quot;);
handle_movement_buttons(400, 550, 100, 40, &quot;Droite&quot;);

if (mouse_b &amp; 1 &amp;&amp; mouse_x &gt;= 10 &amp;&amp; mouse_x &lt;= 210 &amp;&amp; mouse_y &gt;= 650 &amp;&amp;
mouse_y &lt;= 690) {
game_state = STATE_MENU;
rest(200);
}
if (++frame_counter &gt;= 50) {
countdown--;
frame_counter = 0;
}
if (countdown &lt;= 0 || player_moves_left[current_player] &lt;= 0) {
next_turn();
}
}
