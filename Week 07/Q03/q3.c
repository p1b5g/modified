#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void slv_rev(int n) {
    unsigned long long *T3 = calloc((size_t)n + 1, sizeof(unsigned long long));
    unsigned long long *T4 = calloc((size_t)n + 1, sizeof(unsigned long long));
    if (!T3 || !T4) {
        perror("calloc");
        free(T3);
        free(T4);
        return;
    }

    T3[0] = 0;
    for (int i = 1; i <= n; ++i) {
        if (i >= 64) T3[i] = ULLONG_MAX;
        else T3[i] = (1ULL << i) - 1ULL;
    }

    T4[0] = 0;
    for (int i = 1; i <= n; ++i) {
        unsigned long long best = ULLONG_MAX;
        for (int k = 1; k < i; ++k) {
            unsigned long long a = T4[k];
            unsigned long long b = T3[i - k];
            if (a == ULLONG_MAX || b == ULLONG_MAX) continue;
            if (a > ULLONG_MAX / 2) continue;
            unsigned long long cand = 2ULL * a + b;
            if (cand < best) best = cand;
        }
        if (i == 1) best = 1;
        T4[i] = best;
    }

    printf("n = %d disks\n", n);
    if (T4[n] == ULLONG_MAX) printf("Result too large to represent in 64-bit.\n");
    else printf("Minimum moves with 4 pegs (Reve) = %llu\n", T4[n]);

    free(T3);
    free(T4);
}

int main(void) {
    int n = 8;
    slv_rev(n);
    return 0;
}
