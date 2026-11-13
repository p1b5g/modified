#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static void prt_opt(int i, int j, int **s) {
    if (i == j) {
        printf("A%d", i);
        return;
    }
    int k = s[i][j];
    printf("(");
    prt_opt(i, k, s);
    printf(" x ");
    prt_opt(k + 1, j, s);
    printf(")");
}

static long long slv_mcm(int n, const int *p) {
    long long **m = malloc((size_t)(n + 1) * sizeof(long long *));
    if (!m) {
        perror("malloc");
        return -1;
    }

    int **s = malloc((size_t)(n + 1) * sizeof(int *));
    if (!s) {
        perror("malloc");
        free(m);
        return -1;
    }

    for (int i = 0; i <= n; ++i) {
        m[i] = calloc((size_t)(n + 1), sizeof(long long));
        s[i] = calloc((size_t)(n + 1), sizeof(int));
        if (!m[i] || !s[i]) {
            perror("calloc");
            for (int j = 0; j < i; ++j) { free(m[j]); free(s[j]); }
            free(m); free(s);
            return -1;
        }
    }

    for (int i = 1; i <= n; ++i) m[i][i] = 0;
    for (int L = 2; L <= n; ++L) {
        for (int i = 1; i <= n - L + 1; ++i) {
            int j = i + L - 1;
            m[i][j] = LLONG_MAX;
            for (int k = i; k <= j - 1; ++k) {
                long long q = m[i][k] + m[k + 1][j] + (long long)p[i - 1] * p[k] * p[j];
                if (q < m[i][j]) {
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
        }
    }

    long long res = m[1][n];
    printf("Minimum number of scalar multiplications = %lld\n", res);
    printf("Optimal parenthesization (recursive):\n");
    prt_opt(1, n, s);
    printf("\n");

    for (int i = 0; i <= n; ++i) {
        free(m[i]);
        free(s[i]);
    }
    free(m);
    free(s);

    return res;
}

int main(void) {
    int p[] = {30, 35, 15, 5, 10, 20, 25};
    int n = (sizeof(p) / sizeof(p[0])) - 1;

    printf("Matrix Chain Multiplication (n = %d matrices)\nDimensions: ", n);
    for (int i = 0; i <= n; i++) printf("%d ", p[i]);
    printf("\n");

    slv_mcm(n, p);
    return 0;
}
