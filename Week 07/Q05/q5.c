#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned nbr_mv(unsigned b, int n) {
    unsigned nb = 0;
    for (int i = 0; i < n; ++i) if (b & (1u << i)) {
        if (i > 0) nb |= 1u << (i - 1);
        if (i + 1 < n) nb |= 1u << (i + 1);
    }
    return nb;
}

static int max_dep = 12;
static int vis_sz = 0;
static char *memo = NULL;

static int dfs_blf(unsigned b, int n, int depth) {
    if (b == 0) return 1;
    if (depth > max_dep) return 0;
    if (memo[b] != -1) return memo[b];

    for (int p = 0; p < n; ++p) {
        unsigned rem = b & ~(1u << p);
        unsigned next = nbr_mv(rem, n);
        if (dfs_blf(next, n, depth + 1)) {
            memo[b] = 1;
            return 1;
        }
    }
    memo[b] = 0;
    return 0;
}

static int slv_tgt(int n) {
    if (n < 1 || n > 20) {
        printf("n must be between 1 and 20.\n");
        return 1;
    }

    unsigned all = (n == 32) ? 0xFFFFFFFFu : ((1u << n) - 1u);
    vis_sz = 1 << n;
    memo = malloc((size_t)vis_sz * sizeof(char));
    if (!memo) {
        perror("malloc");
        return 1;
    }
    memset(memo, 0xFF, (size_t)vis_sz * sizeof(char));

    int found = 0;
    for (max_dep = 0; max_dep <= 20; ++max_dep) {
        memset(memo, 0xFF, (size_t)vis_sz * sizeof(char));
        if (dfs_blf(all, n, 0)) {
            found = 1;
            break;
        }
    }

    printf("Hiding spots n = %d\n", n);
    if (found) printf("A guaranteed strategy exists within depth %d (search found).\n", max_dep);
    else printf("No guaranteed strategy found up to depth 20.\n");

    free(memo);
    return 0;
}

int main(void) {
    int n = 4;
    return slv_tgt(n);
}
