#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int chk_ks(int S[], int n, int k, int T, int start) {
    if (k == 1) {
        int left = start, right = n - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (S[mid] == T) return 1;
            if (S[mid] < T) left = mid + 1;
            else right = mid - 1;
        }
        return 0;
    }

    if (k == 2) {
        int left = start, right = n - 1;
        while (left < right) {
            int sum = S[left] + S[right];
            if (sum == T) return 1;
            if (sum < T) left++;
            else right--;
        }
        return 0;
    }

    for (int i = start; i <= n - k; i++) {
        if (chk_ks(S, n, k - 1, T - S[i], i + 1))
            return 1;
    }
    return 0;
}

int fnd_ks(int S[], int n, int k, int T) {
    if (k <= 0 || n <= 0 || k > n)
        return 0;

    qsort(S, n, sizeof(int), cmp_int);
    return chk_ks(S, n, k, T, 0);
}

int main(void) {
    int S[] = {3, 11, 7, 1, 15, 9, 4, 12};
    int n = sizeof(S) / sizeof(S[0]);
    int k = 3;
    int T = 23;

    printf("Set S: ");
    for (int i = 0; i < n; i++)
        printf("%d ", S[i]);
    printf("\n");

    printf("k = %d, Target sum T = %d\n", k, T);

    if (fnd_ks(S, n, k, T)) {
        printf("Yes, there exist %d elements that sum to %d.\n", k, T);
    } else {
        printf("No such %d elements sum to %d.\n", k, T);
    }

    return 0;
}
