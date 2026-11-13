#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define MATCH   0
#define INSERT  1
#define DELETE  2

int cmp_lcs(const char X[], const char Y[], int m, int n,
            int dp[][MAX_LEN + 1], int dir[][MAX_LEN + 1]) {
    for (int i = 0; i <= m; i++) dp[i][0] = 0;
    for (int j = 0; j <= n; j++) dp[0][j] = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1]) {
                dp[i][j]  = dp[i - 1][j - 1] + 1;
                dir[i][j] = MATCH;
            } else if (dp[i - 1][j] >= dp[i][j - 1]) {
                dp[i][j]  = dp[i - 1][j];
                dir[i][j] = DELETE;
            } else {
                dp[i][j]  = dp[i][j - 1];
                dir[i][j] = INSERT;
            }
        }
    }

    return dp[m][n];
}

void trc_bck(const char X[], int m, int n,
             int dir[][MAX_LEN + 1], char lcs[]) {
    int idx = 0;
    int i = m, j = n;

    while (i > 0 && j > 0) {
        if (dir[i][j] == MATCH) {
            lcs[idx++] = X[i - 1];
            i--; j--;
        } else if (dir[i][j] == DELETE) {
            i--;
        } else {
            j--;
        }
    }
    lcs[idx] = '\0';

    int left = 0, right = idx - 1;
    while (left < right) {
        char tmp   = lcs[left];
        lcs[left]  = lcs[right];
        lcs[right] = tmp;
        left++; right--;
    }
}

void dsp_dp(const char X[], const char Y[], int m, int n,
            int dp[][MAX_LEN + 1]) {
    printf("\nDP Table (rows = X, cols = Y):\n");
    printf("     ");
    printf("  [0]");
    for (int j = 0; j < n; j++) printf("  [%c]", Y[j]);
    printf("\n");

    for (int i = 0; i <= m; i++) {
        if (i == 0) printf(" [0] ");
        else        printf(" [%c] ", X[i - 1]);

        for (int j = 0; j <= n; j++) {
            printf("  %3d", dp[i][j]);
        }
        printf("\n");
    }
}

int main(void) {
    char X[MAX_LEN + 1] = "ABCBDAB";
    char Y[MAX_LEN + 1] = "BDCABA";

    int m = (int)strlen(X);
    int n = (int)strlen(Y);

    int (*dp)[MAX_LEN + 1]  = malloc((MAX_LEN + 1) * (MAX_LEN + 1) * sizeof(int));
    int (*dir)[MAX_LEN + 1] = malloc((MAX_LEN + 1) * (MAX_LEN + 1) * sizeof(int));
    char lcs[MAX_LEN + 1];

    if (!dp || !dir) {
        free(dp); free(dir);
        return 1;
    }

    int lcs_len = cmp_lcs(X, Y, m, n, dp, dir);
    trc_bck(X, m, n, dir, lcs);

    printf("Results\n");
    printf("Sequence X : \"%s\"  (length %d)\n", X, m);
    printf("Sequence Y : \"%s\"  (length %d)\n", Y, n);
    printf("LCS Length : %d\n", lcs_len);
    printf("LCS String : \"%s\"\n", lcs);

    if (m <= 15 && n <= 15)
        dsp_dp(X, Y, m, n, dp);

    free(dp);
    free(dir);
    return 0;
}
