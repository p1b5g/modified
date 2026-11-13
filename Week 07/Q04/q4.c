#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int ok_tgl(int mask, int n, int i) {
    if (i == n - 1) return 1;
    if (!((mask >> (n - 1 - (i + 1))) & 1)) return 0;
    for (int j = i + 2; j < n; ++j) {
        if ((mask >> (n - 1 - j)) & 1) return 0;
    }
    return 1;
}

static int slv_sw(int n) {
    if (n < 1 || n > 24) {
        printf("n must be between 1 and 24 (practical limit).\n");
        return 1;
    }

    int N = 1 << n;
    int start = N - 1;
    int *dist = malloc(sizeof(int) * N);
    int *par = malloc(sizeof(int) * N);
    int *par_mv = malloc(sizeof(int) * N);
    if (!dist || !par || !par_mv) {
        perror("malloc");
        free(dist); free(par); free(par_mv);
        return 1;
    }

    for (int i = 0; i < N; ++i) {
        dist[i] = -1; par[i] = -1; par_mv[i] = -1;
    }

    int *queue = malloc(sizeof(int) * N);
    if (!queue) {
        perror("malloc");
        free(dist); free(par); free(par_mv);
        return 1;
    }

    int qh = 0, qt = 0;
    queue[qt++] = start; dist[start] = 0;

    while (qh < qt) {
        int mask = queue[qh++];
        if (mask == 0) break;
        for (int i = 0; i < n; ++i) {
            if (!ok_tgl(mask, n, i)) continue;
            int bit = n - 1 - i;
            int nmask = mask ^ (1 << bit);
            if (dist[nmask] == -1) {
                dist[nmask] = dist[mask] + 1;
                par[nmask] = mask;
                par_mv[nmask] = i + 1;
                queue[qt++] = nmask;
            }
        }
    }

    if (dist[0] == -1) {
        printf("No sequence found.\n");
    } else {
        printf("Switches n = %d\n", n);
        printf("Minimal moves = %d\n", dist[0]);
        int p_len = dist[0];
        int *moves = malloc(sizeof(int) * p_len);
        int cur = 0; int p = 0;
        while (cur != start) {
            moves[p++] = par_mv[cur];
            cur = par[cur];
        }
        printf("Sequence of toggles (left-indexed 1..n):\n");
        for (int i = p - 1; i >= 0; --i) printf("%d ", moves[i]);
        printf("\n");
        free(moves);
    }

    free(dist); free(par); free(par_mv); free(queue);
    return 0;
}

int main(void) {
    int n = 4;
    return slv_sw(n);
}
