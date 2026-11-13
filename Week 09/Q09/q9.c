#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N   500
#define MAX_NDS (2 * MAX_N + 5)

typedef struct {
    long long w;
    int term;
    int id;
} SeqNode;

static int p1_l[MAX_NDS], p1_r[MAX_NDS];
static long long p1_w[MAX_NDS];
static int p1_cnt;

static int a_left[MAX_NDS], a_right[MAX_NDS];
static int a_cnt;

static int fnd_min(const SeqNode *seq, int m, int *bi, int *bj) {
    long long best = -1;
    long long run_w = 0;
    int run_idx = -1;
    for (int p = m - 2; p >= 0; p--) {
        int q = p + 1;
        if (seq[q].term || run_idx < 0 || seq[q].w <= run_w) { run_w = seq[q].w; run_idx = q; }
        long long cand = seq[p].w + run_w;
        if (best < 0 || cand <= best) { best = cand; *bi = p; *bj = run_idx; }
    }
    return best >= 0;
}

static int ph1_cmb(const long long *w, int n) {
    if (n < 1) return -1;
    SeqNode *seq = calloc((size_t)n, sizeof(SeqNode));
    p1_cnt = 0;
    for (int i = 0; i < n; i++) {
        p1_l[p1_cnt] = p1_r[p1_cnt] = -1;
        p1_w[p1_cnt] = w[i];
        seq[i] = (SeqNode){w[i], 1, p1_cnt};
        p1_cnt++;
    }
    int m = n;
    while (m > 1) {
        int bi = 0, bj = 1;
        fnd_min(seq, m, &bi, &bj);
        int id = p1_cnt++;
        p1_l[id] = seq[bi].id;
        p1_r[id] = seq[bj].id;
        p1_w[id] = seq[bi].w + seq[bj].w;
        seq[bj] = (SeqNode){p1_w[id], 0, id};
        for (int k = bi; k < m - 1; k++) seq[k] = seq[k + 1];
        m--;
    }
    int root = seq[0].id;
    free(seq);
    return root;
}

static void col_lvl(int node, int depth, int *level) {
    if (p1_l[node] < 0) { level[node] = depth; return; }
    col_lvl(p1_l[node], depth + 1, level);
    col_lvl(p1_r[node], depth + 1, level);
}

static int ph3_reb(const int *level, int n) {
    int *stk_lvl = malloc((size_t)(n + 1) * sizeof(int));
    int *stk_nd = malloc((size_t)(n + 1) * sizeof(int));
    int top = 0;
    a_cnt = n;
    for (int i = 0; i < n; i++) {
        stk_lvl[top] = level[i];
        stk_nd[top] = i;
        top++;
        while (top >= 2 && stk_lvl[top - 1] == stk_lvl[top - 2]) {
            int id = a_cnt++;
            a_left[id] = stk_nd[top - 2];
            a_right[id] = stk_nd[top - 1];
            top -= 2;
            stk_lvl[top] = stk_lvl[top + 1] - 1;
            stk_nd[top] = id;
            top++;
        }
    }
    int root = (top == 1 && stk_lvl[0] == 0) ? stk_nd[0] : -1;
    if (n == 1) root = 0;
    free(stk_lvl);
    free(stk_nd);
    return root;
}

static void prt_tr(int node, int n) {
    if (node < n) { printf("%d", node + 1); return; }
    printf("(");
    prt_tr(a_left[node], n);
    printf(" ");
    prt_tr(a_right[node], n);
    printf(")");
}

static long long opt_alp(const long long *w, int n) {
    long long *pre = calloc((size_t)n + 1, sizeof(long long));
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + w[i];
    long long *cost = calloc((size_t)n * n, sizeof(long long));
    for (int len = 2; len <= n; len++)
        for (int i = 0; i + len - 1 < n; i++) {
            int j = i + len - 1;
            long long best = -1;
            for (int k = i; k < j; k++) {
                long long c = cost[i * n + k] + cost[(k + 1) * n + j];
                if (best < 0 || c < best) best = c;
            }
            cost[i * n + j] = best + pre[j + 1] - pre[i];
        }
    long long ans = cost[0 * n + (n - 1)];
    free(pre);
    free(cost);
    return ans;
}

int main(void) {
    long long w[] = {8, 4, 2, 6, 5, 10};
    int n = sizeof(w) / sizeof(w[0]);
    static int level[MAX_NDS];

    printf("=== Q09: Hu-Tucker Optimal Alphabetic Tree ===\n");
    printf("\nWeights: ");
    for (int i = 0; i < n; i++) printf("%lld ", w[i]);
    printf("\n");

    long long cost = 0;
    int root = -1;
    if (n == 1) {
        level[0] = 0;
        root = 0;
        a_cnt = 1;
    } else {
        int p1_rt = ph1_cmb(w, n);
        col_lvl(p1_rt, 0, level);
        root = ph3_reb(level, n);
    }
    for (int i = 0; i < n; i++) cost += w[i] * level[i];

    printf("\nLeaf levels (depths): ");
    for (int i = 0; i < n; i++) printf("%d ", level[i]);
    printf("\n");

    if (root >= 0) {
        printf("Alphabetic tree (leaf numbers, left-to-right order preserved):\n  ");
        prt_tr(root, n);
        printf("\n");
    }
    printf("\nMinimum weighted path length  sum(w_i * depth_i) = %lld\n", cost);

    printf("\nValidation\n");
    printf("  alphabetic tree rebuilt from the levels : %s\n", root >= 0 ? "PASSED" : "FAILED");
    long long ref = opt_alp(w, n);
    printf("  interval-DP optimum = %lld -> %s\n", ref, ref == cost ? "PASSED" : "FAILED");
    return 0;
}
