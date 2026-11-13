#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

static int min_rms(const int *s, const int *e, int n) {
    int *starts = malloc((size_t)n * sizeof(int));
    int *ends = malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) { starts[i] = s[i]; ends[i] = e[i]; }
    qsort(starts, (size_t)n, sizeof(int), cmp_int);
    qsort(ends, (size_t)n, sizeof(int), cmp_int);

    int rooms = 0, j = 0;
    for (int i = 0; i < n; i++) {
        if (starts[i] < ends[j]) rooms++;
        else j++;
    }
    free(starts);
    free(ends);
    return rooms;
}

static int brt_rms(const int *s, const int *e, int n) {
    int best = 0;
    for (int i = 0; i < n; i++) {
        int c = 0;
        for (int j = 0; j < n; j++)
            if (s[j] <= s[i] && s[i] < e[j]) c++;
        if (c > best) best = c;
    }
    return best;
}

int main(void) {
    int s[] = {0, 5, 15, 10, 20};
    int e[] = {30, 10, 20, 25, 35};
    int n = sizeof(s) / sizeof(s[0]);

    printf("=== Q08: Minimum Number of Meeting Rooms ===\n");
    printf("\nInput (n = %d)\n", n);
    for (int i = 0; i < n; i++) printf("  [%d, %d)\n", s[i], e[i]);

    int ans = min_rms(s, e, n);
    printf("\nMinimum number of rooms required = %d\n", ans);

    int ref = brt_rms(s, e, n);
    printf("Validation (maximum overlap, O(n^2)): %d -> %s\n", ref, ref == ans ? "PASSED" : "FAILED");
    return 0;
}
