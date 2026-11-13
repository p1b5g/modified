#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_C   20
#define MAX_AMT 10000

int min_cns(int coins[], int n, int V, int dp[], int par[]) {
    dp[0]  = 0;
    par[0] = -1;

    for (int v = 1; v <= V; v++) {
        dp[v]  = INT_MAX;
        par[v] = -1;
    }

    for (int v = 1; v <= V; v++) {
        for (int i = 0; i < n; i++) {
            if (coins[i] <= v && dp[v - coins[i]] != INT_MAX) {
                int cand = dp[v - coins[i]] + 1;
                if (cand < dp[v]) {
                    dp[v]  = cand;
                    par[v] = coins[i];
                }
            }
        }
    }

    return (dp[V] == INT_MAX) ? -1 : dp[V];
}

void prt_cns(int par[], int V) {
    if (par[V] == -1) {
        printf("No valid combination exists.\n");
        return;
    }
    printf("Coins used (traceback): ");
    int amt = V;
    while (amt > 0) {
        printf("%d ", par[amt]);
        amt -= par[amt];
    }
    printf("\n");
}

void dsp_dp(int dp[], int V) {
    printf("\nDP Table (amount -> min coins):\n");
    printf("%-8s %-10s\n", "Amount", "Min Coins");
    printf("--\n");
    for (int v = 0; v <= V && v <= 20; v++) {
        if (dp[v] == INT_MAX)
            printf("%-8d %-10s\n", v, "INF");
        else
            printf("%-8d %-10d\n", v, dp[v]);
    }
    if (V > 20) printf("... (showing first 21 rows of %d)\n", V + 1);
}

int main(void) {
    int coins[MAX_C] = {1, 5, 6, 9};
    int n = 4;
    int V = 11;

    int *dp  = (int *)malloc((V + 1) * sizeof(int));
    int *par = (int *)malloc((V + 1) * sizeof(int));

    if (!dp || !par) {
        free(dp); free(par);
        return 1;
    }

    int res = min_cns(coins, n, V, dp, par);

    printf("Results\n");
    printf("Denominations: ");
    for (int i = 0; i < n; i++) printf("%d ", coins[i]);
    printf("\nTarget amount V = %d\n", V);

    if (res == -1) {
        printf("Result: Cannot make amount %d with given denominations.\n", V);
    } else {
        printf("Minimum coins needed: %d\n", res);
        prt_cns(par, V);
    }

    dsp_dp(dp, V);

    free(dp);
    free(par);
    return 0;
}
