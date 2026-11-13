#include <stdio.h>
#include <stdlib.h>

long long fib(int n) {
    if (n <= 1) return n;

    long long *dp = malloc((n + 1) * sizeof(long long));
    if (!dp) return -1;

    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    long long res = dp[n];
    free(dp);
    return res;
}

int main(void) {
    int n = 10;
    printf("The %dth Fibonacci number is: %lld\n", n, fib(n));
    return 0;
}
