#include <allegro.h>
#include "assets.h"
#include "game.h"
#include "menu.h"
#include "utils.h"
#include "movement.h"
#include "spell.h"
#include "images.h"

int main() {
    // Initialisation d'Allegro
    allegro_init();
    install_keyboard();
    install_mouse();
    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800, 700, 0, 0);
    show_mouse(screen);

    // Important : gestion couleur de transparence
    set_color_conversion(COLORCONV_TOTAL);

    // Chargement des ressources visuelles
    load_assets();

    // Initialisation des classes de personnages
    init_classes();

    // Création du buffer principal
    buffer = create_bitmap(800, 700);
    if (!buffer) {
        allegro_message("Erreur : buffer non créé !");
        exit(1);
    }

    // Boucle principale
    while (!key[KEY_ESC]) {
        switch (game_state) {
            case STATE_MENU:
                show_menu();
            break;
            case STATE_RULES:
                show_rules();
            break;
            case STATE_PLAYER_COUNT_SELECTION:
                show_player_count_selection();
            break;
            case STATE_CHARACTER_SELECTION:
                show_character_selection();
            break;
            case STATE_GAME:
                show_game();
            break;
        }

        // Affichage
        blit(buffer, screen, 0, 0, 0, 0, 800, 700);
        clear_bitmap(buffer);
        rest(20);
    }

    // Nettoyage mémoire
    destroy_assets();
    destroy_bitmap(buffer);

    return 0;
}
END_OF_MAIN();
