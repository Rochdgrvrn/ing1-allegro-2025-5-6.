#include "../include/assets.h"
#include <allegro.h>
#include <stdio.h>

// Images principales
BITMAP *img_arena = NULL;
BITMAP *img_characters[4] = {NULL};
BITMAP *img_obstacle = NULL;



// Fonction utilitaire pour les animations
BITMAP *load_bitmap_with_transparency(const char *path) {
    set_color_conversion(COLORCONV_TOTAL);
    BITMAP *original = load_bitmap(path, NULL);
    if (!original) return NULL;

    BITMAP *copy = create_bitmap(original->w, original->h);
    clear_to_color(copy, makecol(255, 0, 255));  // fond magenta
    blit(original, copy, 0, 0, 0, 0, original->w, original->h);
    destroy_bitmap(original);
    return copy;
}

void load_assets() {
    // Arène
    set_color_conversion(COLORCONV_TOTAL);
    img_arena = load_bitmap("../assets/arena.bmp", NULL);
    if (!img_arena) {
        allegro_message("Erreur chargement arène !");
        exit(1);
    }

    // Personnages
    const char *char_paths[4] = {
        "../assets/character_warrior_magenta.bmp",
        "../assets/character_archer_magenta.bmp",
        "../assets/character_mage_magenta.bmp",
        "../assets/character_healer_magenta.bmp"
    };

    for (int i = 0; i < 4; i++) {
        set_color_conversion(COLORCONV_TOTAL);
        img_characters[i] = load_bitmap(char_paths[i], NULL);
        if (!img_characters[i]) {
            allegro_message("Erreur chargement personnage %d", i);
            exit(1);
        }
    }

    // Obstacle (classique sans transparence forcée)
    set_color_conversion(COLORCONV_TOTAL);
    img_obstacle = load_bitmap("../assets/obstacle_rocher.bmp", NULL);
    if (!img_obstacle) {
        allegro_message("Erreur chargement obstacle !");
        exit(1);
    }

    // Animations (avec retrait explicite du fond magenta)
    img_attack_flash = load_bitmap_with_transparency("../assets/attack_flash.bmp");
    if (!img_attack_flash) {
        allegro_message("Erreur chargement animation flash !");
        exit(1);
    }

    img_fireball = load_bitmap_with_transparency("../assets/fireball.bmp");
    if (!img_fireball) {
        allegro_message("Erreur chargement animation fireball !");
        exit(1);
    }

    img_explosion = load_bitmap_with_transparency("../assets/explosion.bmp");
    if (!img_explosion) {
        allegro_message("Erreur chargement animation explosion !");
        exit(1);
    }

    img_heal_wave = load_bitmap_with_transparency("../assets/heal_wave.bmp");
    if (!img_heal_wave) {
        allegro_message("Erreur chargement animation soin !");
        exit(1);
    }
}

void destroy_assets() {
    if (img_arena) {
        destroy_bitmap(img_arena);
        img_arena = NULL;
    }

    for (int i = 0; i < 4; i++) {
        if (img_characters[i]) {
            destroy_bitmap(img_characters[i]);
            img_characters[i] = NULL;
        }
    }

    if (img_obstacle) {
        destroy_bitmap(img_obstacle);
        img_obstacle = NULL;
    }

    if (img_attack_flash) {
        destroy_bitmap(img_attack_flash);
        img_attack_flash = NULL;
    }
    if (img_fireball) {
        destroy_bitmap(img_fireball);
        img_fireball = NULL;
    }
    if (img_explosion) {
        destroy_bitmap(img_explosion);
        img_explosion = NULL;
    }
    if (img_heal_wave) {
        destroy_bitmap(img_heal_wave);
        img_heal_wave = NULL;
    }
}
