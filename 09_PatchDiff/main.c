#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void shuffle(int *a, int n) {
    for (int i = 0; i < n - 1; ++i) {
        int j = i + rand() % (n - i);
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
}

static void dfs(int x, int y, int rooms, int *vis, int G, char *grid, char pass) {
    vis[y * rooms + x] = 1;
    int d[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};
    int o[4] = {0,1,2,3};
    shuffle(o,4);
    for (int k = 0; k < 4; ++k) {
        int i = o[k];
        int nx = x + d[i][0];
        int ny = y + d[i][1];
        if (nx < 0 || nx >= rooms || ny < 0 || ny >= rooms) continue;
        if (vis[ny * rooms + nx]) continue;
        int gx = 2*x+1, gy = 2*y+1;
        int ngx = 2*nx+1, ngy = 2*ny+1;
        int wx = (gx+ngx)/2, wy = (gy+ngy)/2;
        grid[wy*G + wx] = pass;
        dfs(nx,ny,rooms,vis,G,grid,pass);
    }
}

int main(void) {
    int size = 6;
    char pass='.', wall='#';
    srand((unsigned)time(NULL));

    int rooms = size;
    int G = 2 * rooms + 1;

    int *vis = calloc(rooms*rooms, sizeof(int));
    if (!vis) return 1;
    char *grid = malloc(G*G);
    if (!grid) return 1;

    for (int i=0;i<G*G;i++) grid[i]=wall;

    for (int ry=0;ry<rooms;ry++)
        for (int rx=0;rx<rooms;rx++)
            grid[(2*ry+1)*G + (2*rx+1)] = pass;

    dfs(0,0,rooms,vis,G,grid,pass);

    for (int y=0;y<G;y++) {
        for (int x=0;x<G;x++) putchar(grid[y*G+x]);
        putchar('\n');
    }

    free(grid);
    free(vis);
    return 0;
}

