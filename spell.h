#ifndef SPELL_H
#define SPELL_H

typedef struct {
    char name[32];
    int pa_cost;
    int min_range;
    int max_range;
    int min_damage;
    int max_damage;
    int fail_chance_percent;
    int aoe;  // 0 = monocible, 1 = zone
    int heal; // 0 = dégâts, 1 = soin
    int buff_pa; // +PA temporaire
    int buff_pm;
} Spell;

typedef struct {
    char name[32];
    Spell spells[4];
} CharacterClass;

extern CharacterClass all_classes[4];

void init_classes(void);

#endif
