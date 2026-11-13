#include <stdio.h>
#include <stdlib.h>

#define MAX_N 100000

typedef struct {
    long long dist, fuel;
} Station;

typedef struct {
    long long D, F;
    int n;
    Station *st;
} Problem;

static int heap[MAX_N + 5];
static int hp_size;

static int cmp_st(const void *a, const void *b) {
    const Station *p = a, *q = b;
    if (p->dist != q->dist) return (p->dist > q->dist) - (p->dist < q->dist);
    return (p->fuel > q->fuel) - (p->fuel < q->fuel);
}

static void hp_push(const Station *st, int idx) {
    int i = hp_size++;
    heap[i] = idx;
    while (i > 0) {
        int par = (i - 1) / 2;
        if (st[heap[par]].fuel >= st[heap[i]].fuel) break;
        int t = heap[par]; heap[par] = heap[i]; heap[i] = t;
        i = par;
    }
}

static int hp_pop(const Station *st) {
    int top = heap[0];
    heap[0] = heap[--hp_size];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hp_size && st[heap[l]].fuel > st[heap[m]].fuel) m = l;
        if (r < hp_size && st[heap[r]].fuel > st[heap[m]].fuel) m = r;
        if (m == i) break;
        int t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static int min_stp(const Problem *p, int *chosen) {
    long long reach = p->F;
    int stops = 0, i = 0;
    hp_size = 0;
    while (reach < p->D) {
        while (i < p->n && p->st[i].dist <= reach) hp_push(p->st, i++);
        if (hp_size == 0) return -1;
        int s = hp_pop(p->st);
        reach += p->st[s].fuel;
        chosen[stops++] = s;
    }
    return stops;
}

static int dp_stp(const Problem *p) {
    long long *dp = calloc((size_t)p->n + 1, sizeof(long long));
    dp[0] = p->F;
    for (int i = 0; i < p->n; i++)
        for (int j = i + 1; j >= 1; j--)
            if (dp[j - 1] >= p->st[i].dist && dp[j - 1] + p->st[i].fuel > dp[j])
                dp[j] = dp[j - 1] + p->st[i].fuel;
    int ans = -1;
    for (int j = 0; j <= p->n; j++)
        if (dp[j] >= p->D) { ans = j; break; }
    free(dp);
    return ans;
}

int main(void) {
    Station st_arr[] = {{10, 60}, {20, 30}, {30, 30}, {60, 40}};
    Problem p = {100, 10, 4, st_arr};

    printf("=== Q03: Minimum Number of Refuelling Stops ===\n");
    qsort(p.st, (size_t)p.n, sizeof(Station), cmp_st);

    printf("\nInput: target D = %lld, initial fuel F = %lld, stations n = %d\n", p.D, p.F, p.n);
    printf("  (distance, refuel):");
    for (int i = 0; i < p.n; i++) printf(" (%lld,%lld)", p.st[i].dist, p.st[i].fuel);
    printf("\n");

    int *chosen = malloc((size_t)(p.n + 1) * sizeof(int));
    int ans = min_stp(&p, chosen);

    if (ans < 0) printf("\nResult: target is UNREACHABLE (-1)\n");
    else {
        printf("\nResult: minimum refuelling stops = %d\n", ans);
        if (ans > 0) {
            printf("  Stations used (in order of use, distance/fuel):");
            for (int i = 0; i < ans; i++)
                printf(" (%lld,%lld)", p.st[chosen[i]].dist, p.st[chosen[i]].fuel);
            printf("\n");
        }
    }

    int ref = dp_stp(&p);
    printf("\nValidation (O(n^2) dynamic programme): answer = %d -> %s\n", ref,
           ref == ans ? "PASSED" : "FAILED");

    free(chosen);
    return 0;
}
