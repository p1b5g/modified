#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_N   20
#define KEY_LEN 16
#define INF     1e18

double cmp_obs(double p[], double q[], int n,
               double e[][MAX_N + 2], double w[][MAX_N + 2],
               int root[][MAX_N + 2]) {
    for (int i = 0; i <= n; i++) {
        e[i][i]    = q[i];
        w[i][i]    = q[i];
        root[i][i] = 0;
    }

    for (int l = 1; l <= n; l++) {
        for (int i = 0; i <= n - l; i++) {
            int j = i + l;

            e[i][j] = INF;
            w[i][j] = w[i][j - 1] + p[j] + q[j];

            for (int r = i + 1; r <= j; r++) {
                double cost = e[i][r - 1] + e[r][j] + w[i][j];
                if (cost < e[i][j]) {
                    e[i][j]    = cost;
                    root[i][j] = r;
                }
            }
        }
    }

    return e[0][n];
}

void prt_tr(int root[][MAX_N + 2], int i, int j,
            char keys[][KEY_LEN], int depth, int is_left, int par_key) {
    if (j <= i) return;

    int r = root[i][j];
    if (r == 0) return;

    for (int d = 0; d < depth; d++) printf("    ");
    if (depth == 0)
        printf("Root: %s\n", keys[r]);
    else
        printf("%s child of %s: %s\n",
               is_left ? "Left" : "Right", keys[par_key], keys[r]);

    prt_tr(root, i, r - 1, keys, depth + 1, 1, r);
    prt_tr(root, r, j,     keys, depth + 1, 0, r);
}

void norm_pq(double p[], int np, double q[], int nq) {
    double total = 0.0;
    for (int i = 1; i <= np; i++) total += p[i];
    for (int i = 0; i <= nq; i++) total += q[i];
    if (total == 0.0) return;
    for (int i = 1; i <= np; i++) p[i] /= total;
    for (int i = 0; i <= nq; i++) q[i] /= total;
}

void dsp_tbl(double e[][MAX_N + 2], double w[][MAX_N + 2],
             int root[][MAX_N + 2], int n) {
    (void)w;
    printf("\ne[i][j] - Expected search cost:\n");
    printf("     ");
    for (int j = 0; j <= n; j++) printf("  j=%d  ", j);
    printf("\n");
    for (int i = 0; i <= n; i++) {
        printf("i=%d  ", i);
        for (int j = 0; j <= n; j++) {
            if (j < i) printf("       ");
            else       printf(" %5.3f ", e[i][j]);
        }
        printf("\n");
    }

    printf("\nroot[i][j] - Optimal root key index:\n");
    printf("     ");
    for (int j = 1; j <= n; j++) printf("  j=%d ", j);
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("i=%d  ", i);
        for (int j = 1; j <= n; j++) {
            if (j <= i) printf("      ");
            else        printf("   %2d ", root[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    int n = 4;
    double p[MAX_N + 1] = {0.0, 0.15, 0.10, 0.05, 0.10};
    double q[MAX_N + 1] = {0.05, 0.10, 0.05, 0.05, 0.10};
    char keys[MAX_N + 1][KEY_LEN] = {"", "k1", "k2", "k3", "k4"};

    double e[MAX_N + 2][MAX_N + 2];
    double w[MAX_N + 2][MAX_N + 2];
    int root[MAX_N + 2][MAX_N + 2];

    norm_pq(p, n, q, n);
    double min_cst = cmp_obs(p, q, n, e, w, root);

    printf("Results\n");
    printf("Minimum expected search cost: %.4f\n", min_cst);
    printf("\nOptimal BST structure:\n");
    prt_tr(root, 0, n, keys, 0, 0, 0);

    dsp_tbl(e, w, root, n);

    return 0;
}
