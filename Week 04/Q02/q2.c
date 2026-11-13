#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int fnd_sum(int S1[], int S2[], int n, int x) {
    qsort(S1, n, sizeof(int), cmp_int);
    qsort(S2, n, sizeof(int), cmp_int);

    int i = 0;
    int j = n - 1;

    while (i < n && j >= 0) {
        int sum = S1[i] + S2[j];
        if (sum == x) return 1;
        else if (sum < x) i++;
        else j--;
    }
    return 0;
}

void prt_set(int S[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", S[i]);
    }
    printf("\n");
}

int main(void) {
    int S1[] = {10, 4, 8, 15, 3, 20};
    int S2[] = {7, 12, 5, 2, 18, 9};
    int n = sizeof(S1) / sizeof(S1[0]);
    int x = 22;

    printf("Set S1: ");
    prt_set(S1, n);
    printf("Set S2: ");
    prt_set(S2, n);
    printf("Target sum x: %d\n", x);

    if (fnd_sum(S1, S2, n, x)) {
        printf("Pair with sum %d exists.\n", x);
    } else {
        printf("Pair with sum %d does not exist.\n", x);
    }

    return 0;
}
