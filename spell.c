#include <string.h>
#include "../include/spell.h"

CharacterClass all_classes[4];

void init_classes() {
    // Guerrier
    strcpy(all_classes[0].name, "Guerrier");
    all_classes[0].spells[0] = (Spell){"Coup d'épée", 2, 1, 1, 1, 5, 10, 0, 0};
    all_classes[0].spells[1] = (Spell){"Frappe lourde", 4, 1, 2, 4, 8, 20, 0, 0};
    all_classes[0].spells[2] = (Spell){"Hurlement", 3, 0, 0, 0, 0, 0, 0, 1}; // buff (à développer)
    all_classes[0].spells[3] = (Spell){"Tourbillon", 5, 1, 1, 2, 6, 15, 1, 0};
    all_classes[0].spells[2] = (Spell){"Hurlement", 3, 0, 0, 0, 0, 0, 0, 1, 1, 0};

    // Archer
    strcpy(all_classes[1].name, "Archer");
    all_classes[1].spells[0] = (Spell){"Coup de dague", 2, 1, 1, 1, 4, 10, 0, 0};
    all_classes[1].spells[1] = (Spell){"Tir précis", 3, 2, 4, 4, 7, 15, 0, 0};
    all_classes[1].spells[2] = (Spell){"Pluie de flèches", 4, 2, 3, 2, 5, 10, 1, 0};
    all_classes[1].spells[3] = (Spell){"Esquive", 2, 0, 0, 0, 0, 0, 0, 1}; // à personnaliser
    all_classes[1].spells[3] = (Spell){"Esquive", 2, 0, 0, 0, 0, 0, 0, 1, 0, 1};

    // Mage
    strcpy(all_classes[2].name, "Mage");
    all_classes[2].spells[0] = (Spell){"Choc magique", 2, 1, 1, 1, 5, 10, 0, 0};
    all_classes[2].spells[1] = (Spell){"Boule de feu", 4, 3, 5, 4, 9, 25, 1, 0};
    all_classes[2].spells[2] = (Spell){"Gel", 3, 1, 3, 3, 6, 15, 0, 0};
    all_classes[2].spells[3] = (Spell){"Explosion", 6, 1, 3, 6, 10, 30, 1, 0};

    // Soigneur
    strcpy(all_classes[3].name, "Soigneur");
    all_classes[3].spells[0] = (Spell){"Coup de bâton", 2, 1, 1, 1, 4, 10, 0, 0};
    all_classes[3].spells[1] = (Spell){"Soin", 3, 1, 3, 5, 8, 5, 0, 1};
    all_classes[3].spells[2] = (Spell){"Lumière", 4, 2, 4, 3, 6, 10, 1, 1};
    all_classes[3].spells[3] = (Spell){"Purge", 2, 1, 2, 2, 5, 10, 0, 0};
}
