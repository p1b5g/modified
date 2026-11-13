#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000

int cmp_lis(int A[], int n, int dp[], int par[]) {
    int max_len = 1;
    int bst_end = 0;

    for (int i = 0; i < n; i++) {
        dp[i]  = 1;
        par[i] = -1;

        for (int j = 0; j < i; j++) {
            if (A[j] < A[i] && dp[j] + 1 > dp[i]) {
                dp[i]  = dp[j] + 1;
                par[i] = j;
            }
        }

        if (dp[i] > max_len) {
            max_len = dp[i];
            bst_end = i;
        }
    }

    par[n] = bst_end;
    return max_len;
}

void prt_lis(int A[], int n, int par[]) {
    int bst_end = par[n];
    int path[MAX_N];
    int p_len = 0;
    int idx = bst_end;

    while (idx != -1) {
        path[p_len++] = A[idx];
        idx = par[idx];
    }

    printf("LIS elements: [ ");
    for (int i = p_len - 1; i >= 0; i--)
        printf("%d ", path[i]);
    printf("]\n");
}

void dsp_dp(int A[], int dp[], int n) {
    printf("\nDP Table (index -> LIS length ending here):\n");
    printf("%-8s %-8s %-8s\n", "Index", "Value", "dp[i]");
    printf("-\n");
    for (int i = 0; i < n; i++)
        printf("%-8d %-8d %-8d\n", i, A[i], dp[i]);
}

int main(void) {
    int A[MAX_N] = {10, 22, 9, 33, 21, 50, 41, 60, 80};
    int n = 9;

    int *dp  = (int *)malloc(n * sizeof(int));
    int *par = (int *)malloc((n + 1) * sizeof(int));

    if (!dp || !par) {
        free(dp); free(par);
        return 1;
    }

    int lis_len = cmp_lis(A, n, dp, par);

    printf("Results\n");
    printf("Array     : ");
    for (int i = 0; i < n; i++) printf("%d ", A[i]);
    printf("\nLIS Length: %d\n", lis_len);
    prt_lis(A, n, par);

    dsp_dp(A, dp, n);

    free(dp);
    free(par);
    return 0;
}
