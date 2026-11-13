#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

long long cmp_ms(int A[], int n, long long dp[], int par[]) {
    long long max_sum = 0;
    int bst_end = 0;

    for (int i = 0; i < n; i++) {
        dp[i]  = A[i];
        par[i] = -1;

        for (int j = 0; j < i; j++) {
            if (A[j] < A[i] && dp[j] + A[i] > dp[i]) {
                dp[i]  = dp[j] + A[i];
                par[i] = j;
            }
        }

        if (dp[i] > max_sum) {
            max_sum = dp[i];
            bst_end = i;
        }
    }

    par[n] = bst_end;
    return max_sum;
}

void prt_ms(int A[], int n, int par[]) {
    int bst_end = par[n];
    int path[MAX_N];
    int p_len = 0;
    int idx = bst_end;

    while (idx != -1) {
        path[p_len++] = A[idx];
        idx = par[idx];
    }

    printf("Subsequence (max sum): [ ");
    for (int i = p_len - 1; i >= 0; i--)
        printf("%d ", path[i]);
    printf("]\n");
}

void dsp_dp(int A[], long long dp[], int n) {
    printf("\nDP Table (index -> max sum of increasing subseq ending here):\n");
    printf("%-8s %-8s %-12s\n", "Index", "Value", "dp[i] (sum)");
    printf("\n");
    for (int i = 0; i < n; i++)
        printf("%-8d %-8d %-12lld\n", i, A[i], dp[i]);
}

int main(void) {
    int A[MAX_N] = {1, 101, 2, 3, 100, 4, 5};
    int n = 7;

    long long *dp = (long long *)malloc(n * sizeof(long long));
    int *par      = (int *)malloc((n + 1) * sizeof(int));

    if (!dp || !par) {
        free(dp); free(par);
        return 1;
    }

    long long res = cmp_ms(A, n, dp, par);

    printf("Results\n");
    printf("Array      : ");
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\nMax Sum    : %lld\n", res);
    prt_ms(A, n, par);

    dsp_dp(A, dp, n);

    free(dp);
    free(par);
    return 0;
}
