#include <stdio.h>
#include <stdlib.h>

int max_val(int a, int b) {
    return (a > b) ? a : b;
}

int knap(int wts[], int prfs[], int n, int W) {
    int rows = n + 1;
    int cols = W + 1;
    int *dp = calloc(rows * cols, sizeof(int));
    if (!dp) return 0;

    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            int idx = i * cols + w;
            if (i == 0 || w == 0) {
                dp[idx] = 0;
            } else if (wts[i - 1] <= w) {
                int w_out = dp[(i - 1) * cols + w];
                int with = prfs[i - 1] + dp[(i - 1) * cols + (w - wts[i - 1])];
                dp[idx] = max_val(w_out, with);
            } else {
                dp[idx] = dp[(i - 1) * cols + w];
            }
        }
    }

    int res = dp[n * cols + W];
    free(dp);
    return res;
}

int main(void) {
    int wts[] = {1, 3, 4, 5};
    int prfs[] = {10, 40, 50, 70};
    int n = sizeof(wts) / sizeof(wts[0]);
    int W = 8;

    printf("Items (n = %d), Capacity W = %d\n", n, W);
    for (int i = 0; i < n; i++) {
        printf("Item %d: weight = %d, profit = %d\n", i + 1, wts[i], prfs[i]);
    }
    printf("Maximum profit: %d\n", knap(wts, prfs, n, W));
    return 0;
}
