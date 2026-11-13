#include <stdio.h>
#include <stdlib.h>

#define MAX_N 200000

static long long heap[MAX_N + 5];
static int hp_size;

static void hp_push(long long v) {
    int i = hp_size++;
    heap[i] = v;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p] <= heap[i]) break;
        long long t = heap[p]; heap[p] = heap[i]; heap[i] = t;
        i = p;
    }
}

static long long hp_pop(void) {
    long long top = heap[0];
    heap[0] = heap[--hp_size];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hp_size && heap[l] < heap[m]) m = l;
        if (r < hp_size && heap[r] < heap[m]) m = r;
        if (m == i) break;
        long long t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static long long min_cst(const long long *L, int n, int verb) {
    hp_size = 0;
    for (int i = 0; i < n; i++) hp_push(L[i]);
    long long total = 0;
    int step = 1;
    while (hp_size > 1) {
        long long a = hp_pop();
        long long b = hp_pop();
        total += a + b;
        if (verb) printf("  step %2d: join %lld + %lld -> %lld (cost %lld)\n", step, a, b, a + b, a + b);
        step++;
        hp_push(a + b);
    }
    return total;
}

static long long nav_cst(const long long *L, int n) {
    long long *a = malloc((size_t)n * sizeof(long long));
    for (int i = 0; i < n; i++) a[i] = L[i];
    long long total = 0;
    int m = n;
    while (m > 1) {
        int i1 = 0;
        for (int i = 1; i < m; i++) if (a[i] < a[i1]) i1 = i;
        long long x = a[i1];
        a[i1] = a[--m];
        int i2 = 0;
        for (int i = 1; i < m; i++) if (a[i] < a[i2]) i2 = i;
        long long y = a[i2];
        a[i2] = x + y;
        total += x + y;
    }
    free(a);
    return total;
}

int main(void) {
    long long L[] = {2, 4, 3, 8, 5};
    int n = sizeof(L) / sizeof(L[0]);

    printf("=== Q04: Minimum Cost to Connect Sticks ===\n");
    printf("\nInput (n = %d): ", n);
    for (int i = 0; i < n; i++) printf("%lld ", L[i]);
    printf("\n\nMerge sequence\n");

    long long cost = min_cst(L, n, 1);
    printf("\nMinimum total cost = %lld\n", cost);

    long long ref = nav_cst(L, n);
    printf("Validation (naive O(n^2) greedy): cost = %lld -> %s\n", ref,
           ref == cost ? "PASSED" : "FAILED");
    return 0;
}
