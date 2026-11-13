#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int max_val(int a, int b) {
    return (a > b) ? a : b;
}

void lcs(char *X, char *Y) {
    int m = strlen(X);
    int n = strlen(Y);
    int rows = m + 1;
    int cols = n + 1;
    int *dp = calloc(rows * cols, sizeof(int));
    if (!dp) return;

    for (int i = 0; i <= m; i++) dp[i * cols + 0] = 0;
    for (int j = 0; j <= n; j++) dp[0 * cols + j] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i * cols + j] = dp[(i - 1) * cols + (j - 1)] + 1;
            } else {
                dp[i * cols + j] = max_val(dp[(i - 1) * cols + j], dp[i * cols + (j - 1)]);
            }
        }
    }

    int idx = dp[m * cols + n];
    char *lcs_str = malloc(idx + 1);
    if (!lcs_str) { free(dp); return; }
    lcs_str[idx] = '\0';

    int i = m, j = n;
    while (i > 0 && j > 0) {
        if (X[i - 1] == Y[j - 1]) {
            lcs_str[idx - 1] = X[i - 1];
            i--; j--; idx--;
        } else if (dp[(i - 1) * cols + j] > dp[i * cols + (j - 1)]) {
            i--;
        } else {
            j--;
        }
    }

    printf("String X: %s\n", X);
    printf("String Y: %s\n", Y);
    printf("LCS length: %d\n", dp[m * cols + n]);
    printf("LCS: %s\n", lcs_str);
    free(lcs_str);
    free(dp);
}

int main(void) {
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";

    lcs(X, Y);
    return 0;
}
