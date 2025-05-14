#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdbool.h>

typedef struct {
    int x, y;
} Coord;

typedef struct {
    Coord coords[100];
    int length;
    int index;
    bool active;
} MovementPath;

extern MovementPath movement_path;

void highlight_accessible_cells(int start_x, int start_y);
void update_movement(void);
int find_path(int sx, int sy, int dx, int dy, Coord *path, int *path_len);
int is_valid_cell(int x, int y);

#endif

