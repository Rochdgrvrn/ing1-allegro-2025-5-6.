#include <string.h>
#include "movement.h"
#include "game.h"

MovementPath movement_path = {0};
int is_valid_cell(int x, int y) {
    return (x >= 0 && x < 10 && y >= 0 && y < 10 && grid[y][x] == 0);
}


int find_path(int sx, int sy, int dx, int dy, Coord *path, int *path_len) {
    typedef struct { int x, y; Coord prev; } BFSNode;
    BFSNode queue[100];
    Coord parents[10][10];
    bool local_visited[10][10] = {false};

    int front = 0, back = 0;
    queue[back++] = (BFSNode){sx, sy, {-1, -1}};
    local_visited[sy][sx] = true;

    while (front < back) {
        BFSNode cur = queue[front++];
        if (cur.x == dx && cur.y == dy) {
            int len = 0;
            Coord p = (Coord){dx, dy};
            while (!(p.x == sx && p.y == sy)) {
                path[len++] = p;
                p = parents[p.y][p.x];
            }
            path[len++] = (Coord){sx, sy};
            for (int i = 0; i < len / 2; i++) {
                Coord tmp = path[i];
                path[i] = path[len - 1 - i];
                path[len - 1 - i] = tmp;
            }
            *path_len = len;
            return 1;
        }

        int dxs[4] = {1, -1, 0, 0};
        int dys[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dxs[i];
            int ny = cur.y + dys[i];

            if (is_valid_cell(nx, ny) && !local_visited[ny][nx]) {
                local_visited[ny][nx] = true;
                parents[ny][nx] = (Coord){cur.x, cur.y};
                queue[back++] = (BFSNode){nx, ny, {cur.x, cur.y}};
            }
        }
    }
    return 0;
}

void highlight_accessible_cells(int start_x, int start_y) {
    typedef struct { int x, y, dist; } Node;
    Node queue[100];
    int front = 0, back = 0;

    memset(visited, 0, sizeof(visited));
    queue[back++] = (Node){start_x, start_y, 0};
    visited[start_y][start_x] = 1;

    while (front < back) {
        Node cur = queue[front++];
        if (cur.dist >= 3)
            continue;

        int dxs[4] = {0, 0, -1, 1};
        int dys[4] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; i++) {
            int nx = cur.x + dxs[i];
            int ny = cur.y + dys[i];

            if (is_valid_cell(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = 1;
                queue[back++] = (Node){nx, ny, cur.dist + 1};
            }
        }
    }

    // Ne pas surligner la case du joueur
    visited[start_y][start_x] = 0;
}

void update_movement() {
    if (!movement_path.active)
        return;

    if (movement_path.index >= movement_path.length) {
        movement_path.active = false;
        return;
    }

    int *cx = &player_grid_positions[current_player][0];
    int *cy = &player_grid_positions[current_player][1];
    int gx = movement_path.coords[movement_path.index].x;
    int gy = movement_path.coords[movement_path.index].y;

    grid[*cy][*cx] = 0;
    *cx = gx;
    *cy = gy;
    grid[gy][gx] = current_player + 1;

    player_pm_turn[current_player]--;
    player_pm_total[current_player]--;

    movement_path.index++;
    rest(100);
}



