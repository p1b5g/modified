#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100
#define OP_MAT  0
#define OP_SUB  1
#define OP_DEL  2
#define OP_INS  3

int cmp_ed(const char A[], const char B[], int m, int n,
           int dp[][MAX_LEN + 1], int dir[][MAX_LEN + 1]) {
    for (int i = 0; i <= m; i++) { dp[i][0] = i; dir[i][0] = OP_DEL; }
    for (int j = 0; j <= n; j++) { dp[0][j] = j; dir[0][j] = OP_INS; }
    dir[0][0] = OP_MAT;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j]  = dp[i - 1][j - 1];
                dir[i][j] = OP_MAT;
            } else {
                int sub = dp[i - 1][j - 1] + 1;
                int del = dp[i - 1][j]     + 1;
                int ins = dp[i][j - 1]     + 1;

                if (sub <= del && sub <= ins) {
                    dp[i][j]  = sub;
                    dir[i][j] = OP_SUB;
                } else if (del <= ins) {
                    dp[i][j]  = del;
                    dir[i][j] = OP_DEL;
                } else {
                    dp[i][j]  = ins;
                    dir[i][j] = OP_INS;
                }
            }
        }
    }

    return dp[m][n];
}

void trc_bck(const char A[], const char B[], int m, int n,
             int dir[][MAX_LEN + 1]) {
    char op_dtl[2 * MAX_LEN + 5][80];
    int op_cnt = 0;

    int i = m, j = n;
    while (i > 0 || j > 0) {
        switch (dir[i][j]) {
            case OP_MAT:
                snprintf(op_dtl[op_cnt], 80,
                         "KEEP    A[%d]='%c'", i - 1, A[i - 1]);
                i--; j--;
                break;
            case OP_SUB:
                snprintf(op_dtl[op_cnt], 80,
                         "REPLACE A[%d]='%c' -> '%c'", i - 1, A[i - 1], B[j - 1]);
                i--; j--;
                break;
            case OP_DEL:
                snprintf(op_dtl[op_cnt], 80,
                         "DELETE  A[%d]='%c'", i - 1, A[i - 1]);
                i--;
                break;
            case OP_INS:
                snprintf(op_dtl[op_cnt], 80,
                         "INSERT  '%c' before position %d", B[j - 1], i);
                j--;
                break;
            default:
                i--; j--;
                break;
        }
        op_cnt++;
    }

    printf("\nOperation Traceback (forward order):\n");
    printf("%-6s %-s\n", "Step", "Operation");
    for (int k = op_cnt - 1; k >= 0; k--)
        printf("%-6d %s\n", op_cnt - k, op_dtl[k]);
}

void dsp_dp(const char A[], const char B[], int m, int n,
            int dp[][MAX_LEN + 1]) {
    printf("\nEdit Distance DP Table:\n");
    printf("     ");
    printf("  [0]");
    for (int j = 0; j < n; j++) printf("  [%c]", B[j]);
    printf("\n");

    for (int i = 0; i <= m; i++) {
        if (i == 0) printf(" [0] ");
        else        printf(" [%c] ", A[i - 1]);

        for (int j = 0; j <= n; j++)
            printf("  %3d", dp[i][j]);
        printf("\n");
    }
}

int main(void) {
    char A[MAX_LEN + 1] = "kitten";
    char B[MAX_LEN + 1] = "sitting";

    int m = (int)strlen(A);
    int n = (int)strlen(B);

    int (*dp)[MAX_LEN + 1]  = malloc((MAX_LEN + 1) * (MAX_LEN + 1) * sizeof(int));
    int (*dir)[MAX_LEN + 1] = malloc((MAX_LEN + 1) * (MAX_LEN + 1) * sizeof(int));

    if (!dp || !dir) {
        free(dp); free(dir);
        return 1;
    }

    int dist = cmp_ed(A, B, m, n, dp, dir);

    printf("Results\n");
    printf("String A   : \"%s\"  (length %d)\n", A, m);
    printf("String B   : \"%s\"  (length %d)\n", B, n);
    printf("Edit Distance: %d\n", dist);

    trc_bck(A, B, m, n, dir);

    if (m <= 12 && n <= 12)
        dsp_dp(A, B, m, n, dp);

    free(dp);
    free(dir);
    return 0;
}
