#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int max_val(int a, int b) {
    return (a > b) ? a : b;
}

int egg_dp(int E, int F) {
    int **dp = (int **)malloc((E + 1) * sizeof(int *));
    for (int i = 0; i <= E; i++) {
        dp[i] = (int *)calloc((F + 1), sizeof(int));
    }

    for (int i = 1; i <= E; i++) {
        dp[i][0] = 0;
        dp[i][1] = 1;
    }

    for (int j = 1; j <= F; j++) {
        dp[1][j] = j;
    }

    for (int i = 2; i <= E; i++) {
        for (int j = 2; j <= F; j++) {
            dp[i][j] = INT_MAX;

            for (int x = 1; x <= j; x++) {
                int res = 1 + max_val(dp[i - 1][x - 1], dp[i][j - x]);

                if (res < dp[i][j]) {
                    dp[i][j] = res;
                }
            }
        }
    }

    int res = dp[E][F];

    for (int i = 0; i <= E; i++) {
        free(dp[i]);
    }
    free(dp);

    return res;
}

int main(void) {
    int eggs = 2;
    int floors = 100;

    printf("Super egg testing experiment (E = %d, F = %d)\n", eggs, floors);
    int min_tr = egg_dp(eggs, floors);
    printf("Minimum number of droppings guaranteed to find the highest safe floor: %d\n", min_tr);

    return 0;
}
