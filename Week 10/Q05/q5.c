#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int u;
    int v;
} SwpEdge;

typedef struct {
    int *par;
    int *rank;
    int n;
} DSU;

DSU *crt_dsu(int n) {
    DSU *dsu = (DSU *)malloc(sizeof(DSU));
    dsu->n = n;
    dsu->par = (int *)malloc((n + 1) * sizeof(int));
    dsu->rank = (int *)calloc((n + 1), sizeof(int));

    for (int i = 1; i <= n; i++) {
        dsu->par[i] = i;
    }
    return dsu;
}

void fre_dsu(DSU *dsu) {
    if (dsu) {
        free(dsu->par);
        free(dsu->rank);
        free(dsu);
    }
}

int fnd_set(DSU *dsu, int x) {
    if (dsu->par[x] != x) {
        dsu->par[x] = fnd_set(dsu, dsu->par[x]);
    }
    return dsu->par[x];
}

void uni_set(DSU *dsu, int x, int y) {
    int root_x = fnd_set(dsu, x);
    int root_y = fnd_set(dsu, y);

    if (root_x == root_y) {
        return;
    }

    if (dsu->rank[root_x] < dsu->rank[root_y]) {
        dsu->par[root_x] = root_y;
    } else if (dsu->rank[root_x] > dsu->rank[root_y]) {
        dsu->par[root_y] = root_x;
    } else {
        dsu->par[root_y] = root_x;
        dsu->rank[root_x]++;
    }
}

int cmp_asc(const void *a, const void *b) {
    int val_a = *(const int *)a;
    int val_b = *(const int *)b;
    return (val_a > val_b) - (val_a < val_b);
}

void min_prm(int n, int m, const int *A, const SwpEdge *swaps) {
    DSU *dsu = crt_dsu(n);

    for (int i = 0; i < m; i++) {
        if (swaps[i].u >= 1 && swaps[i].u <= n && swaps[i].v >= 1 && swaps[i].v <= n) {
            uni_set(dsu, swaps[i].u, swaps[i].v);
        }
    }

    int *cmp_sz = (int *)calloc(n + 1, sizeof(int));
    for (int i = 1; i <= n; i++) {
        cmp_sz[fnd_set(dsu, i)]++;
    }

    int *cmp_off = (int *)calloc(n + 2, sizeof(int));
    for (int r = 1; r <= n; r++) {
        cmp_off[r + 1] = cmp_off[r] + cmp_sz[r];
    }

    int *wrt_pos = (int *)malloc((n + 1) * sizeof(int));
    for (int r = 1; r <= n; r++) {
        wrt_pos[r] = cmp_off[r];
    }

    int *grp_idx = (int *)malloc(n * sizeof(int));
    int *grp_val = (int *)malloc(n * sizeof(int));

    for (int i = 1; i <= n; i++) {
        int r = fnd_set(dsu, i);
        int pos = wrt_pos[r]++;
        grp_idx[pos] = i;
        grp_val[pos] = A[i];
    }

    int *res = (int *)malloc((n + 1) * sizeof(int));
    for (int r = 1; r <= n; r++) {
        int count = cmp_sz[r];
        if (count > 0) {
            int start = cmp_off[r];
            qsort(&grp_val[start], count, sizeof(int), cmp_asc);
            for (int k = 0; k < count; k++) {
                res[grp_idx[start + k]] = grp_val[start + k];
            }
        }
    }

    printf("\n--- Lexicographically Smallest Permutation Result ---\n");
    printf("Initial Permutation A : [");
    for (int i = 1; i <= n; i++) {
        printf("%d%s", A[i], (i == n) ? "]\n" : ", ");
    }

    printf("Smallest Permutation  : [");
    for (int i = 1; i <= n; i++) {
        printf("%d%s", res[i], (i == n) ? "]\n" : ", ");
    }

    free(cmp_sz);
    free(cmp_off);
    free(wrt_pos);
    free(grp_idx);
    free(grp_val);
    free(res);
    fre_dsu(dsu);
}

int main(void) {
    int n = 6;
    int A[] = {0, 5, 3, 1, 6, 4, 2};
    SwpEdge swaps[] = {{1, 3}, {3, 5}, {2, 6}, {4, 6}};
    int m = sizeof(swaps) / sizeof(swaps[0]);

    printf("=============================================================\n");
    printf(" Q05: Lexicographically Smallest Connected Component Perm.\n");
    printf("=============================================================\n");

    min_prm(n, m, A, swaps);
    return EXIT_SUCCESS;
}
