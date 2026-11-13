#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 12
#define MAX_LEN 24
#define BUF     512

typedef struct {
    int n;
    char s[MAX_STR][MAX_LEN + 2];
} StrSet;

static int overlap(const char *a, const char *b) {
    int la = (int)strlen(a), lb = (int)strlen(b);
    int m = (la < lb ? la : lb) - 1;
    for (int l = m; l >= 1; l--)
        if (strncmp(a + la - l, b, (size_t)l) == 0) return l;
    return 0;
}

static void red_set(const StrSet *in, StrSet *out) {
    out->n = 0;
    for (int i = 0; i < in->n; i++) {
        int drop = 0;
        for (int j = 0; j < in->n && !drop; j++) {
            if (i == j) continue;
            if (strstr(in->s[j], in->s[i]) != NULL) {
                if (strcmp(in->s[i], in->s[j]) != 0) drop = 1;
                else if (j < i) drop = 1;
            }
        }
        if (!drop) strcpy(out->s[out->n++], in->s[i]);
    }
}

static void grd_sup(const StrSet *r, char *res) {
    int n = r->n;
    static char buf[MAX_STR][BUF];
    int ov[MAX_STR][MAX_STR], active[MAX_STR];
    for (int i = 0; i < n; i++) { strcpy(buf[i], r->s[i]); active[i] = 1; }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ov[i][j] = (i == j) ? 0 : overlap(r->s[i], r->s[j]);

    for (int mrg = 0; mrg < n - 1; mrg++) {
        int bi = -1, bj = -1, best = -1;
        for (int i = 0; i < n; i++) {
            if (!active[i]) continue;
            for (int j = 0; j < n; j++) {
                if (i == j || !active[j]) continue;
                if (ov[i][j] > best) { best = ov[i][j]; bi = i; bj = j; }
            }
        }
        char merged[BUF];
        snprintf(merged, sizeof merged, "%s%s", buf[bi], buf[bj] + best);
        strcpy(buf[bi], merged);
        active[bj] = 0;
        for (int x = 0; x < n; x++) {
            if (!active[x] || x == bi) continue;
            ov[bi][x] = ov[bj][x];
        }
    }
    for (int i = 0; i < n; i++)
        if (active[i]) { strcpy(res, buf[i]); return; }
    res[0] = '\0';
}

static void opt_sup(const StrSet *r, char *res) {
    int n = r->n;
    static int dp[1 << MAX_STR][MAX_STR], par[1 << MAX_STR][MAX_STR];
    int ov[MAX_STR][MAX_STR];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) ov[i][j] = (i == j) ? 0 : overlap(r->s[i], r->s[j]);

    int full = (1 << n) - 1;
    for (int m = 0; m <= full; m++)
        for (int i = 0; i < n; i++) { dp[m][i] = -1; par[m][i] = -1; }
    for (int i = 0; i < n; i++) dp[1 << i][i] = 0;

    for (int m = 1; m <= full; m++)
        for (int i = 0; i < n; i++) {
            if (dp[m][i] < 0) continue;
            for (int j = 0; j < n; j++) {
                if (m & (1 << j)) continue;
                int nm = m | (1 << j), v = dp[m][i] + ov[i][j];
                if (v > dp[nm][j]) { dp[nm][j] = v; par[nm][j] = i; }
            }
        }

    int last = 0;
    for (int i = 1; i < n; i++) if (dp[full][i] > dp[full][last]) last = i;

    int order[MAX_STR], cnt = 0, m = full, cur = last;
    while (cur >= 0) {
        order[cnt++] = cur;
        int p = par[m][cur];
        m ^= (1 << cur);
        cur = p;
    }
    strcpy(res, r->s[order[cnt - 1]]);
    for (int k = cnt - 2; k >= 0; k--) {
        int o = ov[order[k + 1]][order[k]];
        strcat(res, r->s[order[k]] + o);
    }
}

static int has_all(const StrSet *in, const char *sup) {
    for (int i = 0; i < in->n; i++)
        if (strstr(sup, in->s[i]) == NULL) return 0;
    return 1;
}

int main(void) {
    StrSet in = {5, {"catg", "ctaagt", "gcta", "ttca", "atgcatc"}};
    StrSet red;
    char greedy[BUF], opt[BUF];

    printf("=== Q10: Greedy Superstring Conjecture ===\n");
    printf("\nInput strings (n = %d):", in.n);
    for (int i = 0; i < in.n; i++) printf(" %s", in.s[i]);
    printf("\n");

    red_set(&in, &red);
    printf("After removing duplicates / substrings (n = %d):", red.n);
    for (int i = 0; i < red.n; i++) printf(" %s", red.s[i]);
    printf("\n");

    grd_sup(&red, greedy);
    opt_sup(&red, opt);

    int lg = (int)strlen(greedy), lo = (int)strlen(opt);
    printf("\nGreedy superstring  (length %2d): %s\n", lg, greedy);
    printf("Optimal superstring (length %2d): %s\n", lo, opt);
    printf("Approximation ratio |greedy| / |optimal| = %.4f\n", (double)lg / (double)lo);
    printf("Conjectured bound: ratio <= 2   -> %s on this instance\n", lg <= 2 * lo ? "holds" : "VIOLATED");

    printf("\nValidation\n");
    printf("  greedy result contains every input string  : %s\n", has_all(&in, greedy) ? "PASSED" : "FAILED");
    printf("  optimal result contains every input string : %s\n", has_all(&in, opt) ? "PASSED" : "FAILED");
    printf("  |optimal| <= |greedy|                      : %s\n", lo <= lg ? "PASSED" : "FAILED");
    return 0;
}
