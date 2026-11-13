#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int r;
    int c;
} Cell;

typedef struct {
    int *par;
    int *rank;
    bool *is_land;
    int rows;
    int cols;
    int isl_cnt;
} GridDSU;

GridDSU *crt_dsu(int R, int C) {
    int tot_cel = R * C;
    GridDSU *dsu = (GridDSU *)malloc(sizeof(GridDSU));
    dsu->rows = R;
    dsu->cols = C;
    dsu->isl_cnt = 0;
    dsu->par = (int *)malloc(tot_cel * sizeof(int));
    dsu->rank = (int *)calloc(tot_cel, sizeof(int));
    dsu->is_land = (bool *)calloc(tot_cel, sizeof(bool));

    for (int i = 0; i < tot_cel; i++) {
        dsu->par[i] = i;
    }
    return dsu;
}

void fre_dsu(GridDSU *dsu) {
    if (dsu) {
        free(dsu->par);
        free(dsu->rank);
        free(dsu->is_land);
        free(dsu);
    }
}

int flt_crd(int r, int c, int C) {
    return r * C + c;
}

int fnd_set(GridDSU *dsu, int idx) {
    if (dsu->par[idx] != idx) {
        dsu->par[idx] = fnd_set(dsu, dsu->par[idx]);
    }
    return dsu->par[idx];
}

bool uni_set(GridDSU *dsu, int idx1, int idx2) {
    int root1 = fnd_set(dsu, idx1);
    int root2 = fnd_set(dsu, idx2);

    if (root1 == root2) {
        return false;
    }

    if (dsu->rank[root1] < dsu->rank[root2]) {
        dsu->par[root1] = root2;
    } else if (dsu->rank[root1] > dsu->rank[root2]) {
        dsu->par[root2] = root1;
    } else {
        dsu->par[root2] = root1;
        dsu->rank[root1]++;
    }
    return true;
}

int add_lnd(GridDSU *dsu, int r, int c) {
    int idx = flt_crd(r, c, dsu->cols);

    if (dsu->is_land[idx]) {
        return dsu->isl_cnt;
    }

    dsu->is_land[idx] = true;
    dsu->isl_cnt++;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++) {
        int nr = r + dr[d];
        int nc = c + dc[d];

        if (nr >= 0 && nr < dsu->rows && nc >= 0 && nc < dsu->cols) {
            int n_idx = flt_crd(nr, nc, dsu->cols);
            if (dsu->is_land[n_idx]) {
                if (uni_set(dsu, idx, n_idx)) {
                    dsu->isl_cnt--;
                }
            }
        }
    }

    return dsu->isl_cnt;
}

void sim_ops(int R, int C, int K, Cell *ops) {
    GridDSU *dsu = crt_dsu(R, C);

    printf("\n--- Dynamic Connected Components (Islands) Log ---\n");
    for (int i = 0; i < K; i++) {
        int r = ops[i].r;
        int c = ops[i].c;

        if (r < 0 || r >= R || c < 0 || c >= C) {
            printf("Operation %2d: Add Land (%d, %d) -> OUT OF BOUNDS | Current Islands = %d\n",
                   i + 1, r, c, dsu->isl_cnt);
            continue;
        }

        int cur_isl = add_lnd(dsu, r, c);
        printf("Operation %2d: Add Land (%d, %d) -> Current Number of Islands = %d\n",
               i + 1, r, c, cur_isl);
    }

    fre_dsu(dsu);
}

int main(void) {
    int R = 3, C = 3;
    Cell ops[] = {{0, 0}, {0, 1}, {1, 2}, {2, 1}, {1, 1}};
    int K = sizeof(ops) / sizeof(ops[0]);

    printf("=============================================================\n");
    printf(" Q02: Dynamic Connected Components in a Grid (Islands II)\n");
    printf("=============================================================\n");

    sim_ops(R, C, K, ops);
    return EXIT_SUCCESS;
}
