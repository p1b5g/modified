#include <stdio.h>
#include <stdlib.h>

#define MAX_ROD 100

int rod_cut(int P[], int n, int dp[], int cut[]) {
    dp[0]  = 0;
    cut[0] = 0;

    for (int l = 1; l <= n; l++) {
        int best = -1;
        for (int k = 1; k <= l; k++) {
            int rev = P[k] + dp[l - k];
            if (rev > best) {
                best   = rev;
                cut[l] = k;
            }
        }
        dp[l] = best;
    }

    return dp[n];
}

void prt_cut(int cut[], int n) {
    printf("Optimal cuts: [ ");
    while (n > 0) {
        printf("%d ", cut[n]);
        n -= cut[n];
    }
    printf("]\n");
}

void dsp_dp(int dp[], int cut[], int n) {
    printf("\nDP Table:\n");
    printf("%-8s %-14s %-10s\n", "Length", "Max Revenue", "First Cut");
    for (int l = 0; l <= n; l++)
        printf("%-8d %-14d %-10d\n", l, dp[l], cut[l]);
}

int main(void) {
    int P[MAX_ROD + 1] = {0, 1, 5, 8, 9, 10, 17, 17, 20};
    int n = 8;

    int *dp  = (int *)malloc((n + 1) * sizeof(int));
    int *cut = (int *)malloc((n + 1) * sizeof(int));

    if (!dp || !cut) {
        free(dp); free(cut);
        return 1;
    }

    int max_rev = rod_cut(P, n, dp, cut);

    printf("Results\n");
    printf("Rod length      : %d\n", n);
    printf("Maximum revenue : %d\n", max_rev);
    prt_cut(cut, n);

    dsp_dp(dp, cut, n);

    free(dp);
    free(cut);
    return 0;
}
