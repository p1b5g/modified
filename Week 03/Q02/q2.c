#include <stdio.h>
#include <stdlib.h>

int weigh(int coins[], int l_start, int l_end, int r_start, int r_end) {
    int l_wt = 0;
    int r_wt = 0;

    for (int i = l_start; i <= l_end; i++) {
        l_wt += coins[i];
    }
    for (int i = r_start; i <= r_end; i++) {
        r_wt += coins[i];
    }

    if (l_wt < r_wt) return -1;
    if (l_wt > r_wt) return 1;
    return 0;
}

int src_def(int coins[], int low, int high, int kn_good) {
    int n = high - low + 1;

    if (n == 1) {
        if (kn_good != -1) {
            int res = weigh(coins, low, low, kn_good, kn_good);
            if (res < 0) {
                return low;
            } else {
                return -1;
            }
        }
        return -1;
    }

    int half_sz = n / 2;
    int mid = low + half_sz - 1;

    int l_start = low;
    int l_end = mid;
    int r_start = mid + 1;
    int r_end = mid + half_sz;

    int res = weigh(coins, l_start, l_end, r_start, r_end);

    if (res == 0) {
        kn_good = l_start;
        if (n % 2 != 0) {
            return src_def(coins, high, high, kn_good);
        } else {
            return -1;
        }
    } else if (res < 0) {
        kn_good = r_start;
        return src_def(coins, l_start, l_end, kn_good);
    } else {
        kn_good = l_start;
        return src_def(coins, r_start, r_end, kn_good);
    }
}

int fnd_def(int coins[], int n) {
    if (n < 2) {
        printf("Need at least 2 coins to find a defective one by comparison.\n");
        return -1;
    }
    return src_def(coins, 0, n - 1, -1);
}

int main(void) {
    int coins[] = {10, 10, 10, 9, 10, 10, 10, 10};
    int n = sizeof(coins) / sizeof(coins[0]);

    printf("Coin weights: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", coins[i]);
    }
    printf("\n");

    int res = fnd_def(coins, n);

    if (res != -1) {
        printf("Defective coin found at index %d (Weight: %d)\n", res, coins[res]);
    } else {
        printf("No defective coin found.\n");
    }

    return 0;
}
