#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int mcm(int arr[], int n) {
    int *dp = malloc(n * n * sizeof(int));
    if (!dp) return 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) dp[i * n + j] = 0;
    }

    for (int i = 1; i < n; i++) {
        dp[i * n + i] = 0;
    }

    for (int len = 2; len < n; len++) {
        for (int i = 1; i < n - len + 1; i++) {
            int j = i + len - 1;
            dp[i * n + j] = INT_MAX;
            for (int k = i; k < j; k++) {
                int cost = dp[i * n + k] + dp[(k + 1) * n + j] + arr[i - 1] * arr[k] * arr[j];
                if (cost < dp[i * n + j]) {
                    dp[i * n + j] = cost;
                }
            }
        }
    }

    int res = dp[1 * n + (n - 1)];
    free(dp);
    return res;
}

int main(void) {
    int arr[] = {10, 30, 5, 60};
    int dims = sizeof(arr) / sizeof(arr[0]);

    printf("Dimensions:");
    for (int i = 0; i < dims; i++) printf(" %d", arr[i]);
    printf("\nMinimum scalar multiplications: %d\n", mcm(arr, dims));
    return 0;
}
