#include <stdio.h>
#include <stdlib.h>

#define MAX_N 1000000

static long long cnd_slp(const int *r, int n) {
    long long total = 1;
    long long up = 0, down = 0, peak = 0;
    for (int i = 1; i < n; i++) {
        if (r[i] > r[i - 1]) {
            up++;
            peak = up;
            down = 0;
            total += 1 + up;
        } else if (r[i] == r[i - 1]) {
            up = down = peak = 0;
            total += 1;
        } else {
            up = 0;
            down++;
            total += 1 + down;
            if (peak >= down) total--;
        }
    }
    return total;
}

static long long cnd_2ps(const int *r, int n, int *candy) {
    for (int i = 0; i < n; i++) candy[i] = 1;
    for (int i = 1; i < n; i++)
        if (r[i] > r[i - 1]) candy[i] = candy[i - 1] + 1;
    for (int i = n - 2; i >= 0; i--)
        if (r[i] > r[i + 1] && candy[i] <= candy[i + 1]) candy[i] = candy[i + 1] + 1;
    long long total = 0;
    for (int i = 0; i < n; i++) total += candy[i];
    return total;
}

static int dst_ok(const int *r, int n, const int *candy) {
    for (int i = 0; i < n; i++) {
        if (candy[i] < 1) return 0;
        if (i > 0 && r[i] > r[i - 1] && candy[i] <= candy[i - 1]) return 0;
        if (i + 1 < n && r[i] > r[i + 1] && candy[i] <= candy[i + 1]) return 0;
    }
    return 1;
}

int main(void) {
    int rating[] = {1, 0, 2, 4, 3, 2, 1, 5};
    int n = sizeof(rating) / sizeof(rating[0]);
    static int candy[MAX_N + 5];

    printf("=== Q05: Candy Distribution (Bi-directional Slope Greedy) ===\n");
    printf("\nRatings: ");
    for (int i = 0; i < n; i++) printf("%d ", rating[i]);
    printf("\n");

    long long ans = cnd_slp(rating, n);
    printf("\nMinimum total candies (slope method) = %lld\n", ans);

    long long ref = cnd_2ps(rating, n, candy);
    printf("Candies per child (two-pass) : ");
    for (int i = 0; i < n; i++) printf("%d ", candy[i]);
    printf("\n");

    int valid = dst_ok(rating, n, candy);
    printf("\nValidation\n");
    printf("  two-pass total = %lld -> %s\n", ref, ref == ans ? "PASSED (totals agree)" : "FAILED");
    printf("  distribution satisfies all neighbour rules -> %s\n", valid ? "PASSED" : "FAILED");
    return 0;
}
