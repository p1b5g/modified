#include <stdio.h>
#include <stdlib.h>

#define MAX_C   20
#define MAX_AMT 1000

long long cnt_wys(int coins[], int n, int V, long long dp[]) {
    dp[0] = 1;

    for (int v = 1; v <= V; v++)
        dp[v] = 0;

    for (int i = 0; i < n; i++) {
        for (int v = coins[i]; v <= V; v++) {
            dp[v] += dp[v - coins[i]];
        }
    }

    return dp[V];
}

void dsp_dp(long long dp[], int V) {
    printf("\nDP Table (amount -> number of combinations):\n");
    printf("%-8s %-15s\n", "Amount", "Combinations");
    printf("\n");
    int limit = (V < 20) ? V : 20;
    for (int v = 0; v <= limit; v++)
        printf("%-8d %-15lld\n", v, dp[v]);
    if (V > 20) printf("... (showing first 21 rows of %d)\n", V + 1);
}

int main(void) {
    int coins[MAX_C] = {1, 2, 5};
    int n = 3;
    int V = 10;

    long long *dp = (long long *)malloc((V + 1) * sizeof(long long));
    if (!dp) {
        return 1;
    }

    long long res = cnt_wys(coins, n, V, dp);

    printf("Results\n");
    printf("Denominations: ");
    for (int i = 0; i < n; i++) printf("%d ", coins[i]);
    printf("\nTarget amount V = %d\n", V);
    printf("Total distinct combinations: %lld\n", res);

    dsp_dp(dp, V);

    free(dp);
    return 0;
}
