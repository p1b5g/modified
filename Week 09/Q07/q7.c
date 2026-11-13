#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000

static long long heap[MAX_N + 5];
static int hp_size;

static void hp_push(long long v) {
    int i = hp_size++;
    heap[i] = v;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[p] >= heap[i]) break;
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
        if (l < hp_size && heap[l] > heap[m]) m = l;
        if (r < hp_size && heap[r] > heap[m]) m = r;
        if (m == i) break;
        long long t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static long long min_dev(const long long *a, int n) {
    hp_size = 0;
    long long mn = -1;
    for (int i = 0; i < n; i++) {
        long long v = (a[i] % 2) ? a[i] * 2 : a[i];
        if (mn < 0 || v < mn) mn = v;
        hp_push(v);
    }
    long long best = heap[0] - mn;
    for (;;) {
        long long mx = heap[0];
        if (mx - mn < best) best = mx - mn;
        if (mx % 2) break;
        hp_pop();
        mx /= 2;
        if (mx < mn) mn = mx;
        hp_push(mx);
    }
    return best;
}

static int bld_cnd(long long x, long long *cand) {
    long long o = x;
    int k = 0;
    while (o % 2 == 0) { o /= 2; k++; }
    int top = k > 1 ? k : 1;
    int c = 0;
    for (int j = 0; j <= top; j++) cand[c++] = o << j;
    return c;
}

static long long brt_dev(const long long *a, int n) {
    long long (*cand)[40] = malloc((size_t)n * sizeof *cand);
    int *cn = malloc((size_t)n * sizeof(int));
    for (int i = 0; i < n; i++) cn[i] = bld_cnd(a[i], cand[i]);

    long long best = -1;
    for (int e = 0; e < n; e++)
        for (int c = 0; c < cn[e]; c++) {
            long long m = cand[e][c], mx = m;
            int ok = 1;
            for (int i = 0; i < n && ok; i++) {
                long long pick = -1;
                for (int j = 0; j < cn[i]; j++)
                    if (cand[i][j] >= m && (pick < 0 || cand[i][j] < pick)) pick = cand[i][j];
                if (pick < 0) ok = 0;
                else if (pick > mx) mx = pick;
            }
            if (ok && (best < 0 || mx - m < best)) best = mx - m;
        }
    free(cand);
    free(cn);
    return best;
}

int main(void) {
    long long a[] = {1, 2, 3, 4};
    int n = sizeof(a) / sizeof(a[0]);

    printf("=== Q07: Minimise Deviation in Array ===\n");
    printf("\nArray: ");
    for (int i = 0; i < n; i++) printf("%lld ", a[i]);
    printf("\n");

    long long ans = min_dev(a, n);
    printf("\nMinimum deviation = %lld\n", ans);

    long long ref = brt_dev(a, n);
    printf("Validation (exhaustive candidate window): %lld -> %s\n", ref,
           ref == ans ? "PASSED" : "FAILED");
    return 0;
}
