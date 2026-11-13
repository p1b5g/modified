#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_N 1000
#define EPS   1e-9

typedef struct {
    double v, w, lam, d;
    double x;
    double g;
    int full;
    int member;
} Item;

typedef struct {
    double lam, d, x;
    int id;
} Slot;

static double rnd_rng(double lo, double hi) {
    return lo + (hi - lo) * ((double)rand() / (double)RAND_MAX);
}

static int cmp_slt(const void *a, const void *b) {
    const Slot *p = a, *q = b;
    if (p->lam > q->lam) return -1;
    if (p->lam < q->lam) return 1;
    return p->id - q->id;
}

static void bld_slt(const Item *it, int n, const double *x, Slot *s) {
    for (int i = 0; i < n; i++) {
        s[i].lam = it[i].lam;
        s[i].d   = it[i].d;
        s[i].x   = x[i];
        s[i].id  = i;
    }
    qsort(s, (size_t)n, sizeof(Slot), cmp_slt);
}

static double sch_val(const Item *it, int n, const double *x) {
    Slot *s = malloc((size_t)n * sizeof(Slot));
    bld_slt(it, n, x, s);
    double t = 0, total = 0;
    for (int i = 0; i < n; i++) {
        total += s[i].d * s[i].x - s[i].lam * (t * s[i].x + 0.5 * s[i].x * s[i].x);
        t += s[i].x;
    }
    free(s);
    return total;
}

static void ini_st(Item *it, int n) {
    for (int i = 0; i < n; i++) {
        it[i].d      = it[i].v / it[i].w;
        it[i].x      = 0;
        it[i].g      = it[i].d;
        it[i].full   = 0;
        it[i].member = 0;
    }
}

static int rfl_ldr(Item *it, int n) {
    double best = -1e300;
    for (int i = 0; i < n; i++)
        if (!it[i].full && it[i].g > best) best = it[i].g;
    if (best <= EPS) return 0;
    for (int i = 0; i < n; i++)
        if (!it[i].full && it[i].g >= best - 1e-7 * (1.0 + fabs(best))) it[i].member = 1;
    return 1;
}

static double slv_knp(Item *it, int n, double W) {
    double used = 0;
    ini_st(it, n);
    for (;;) {
        int k = -1;
        for (int i = 0; i < n; i++)
            if (it[i].member && !it[i].full && (k < 0 || it[i].lam < it[k].lam)) k = i;
        if (k < 0) {
            if (!rfl_ldr(it, n)) break;
            continue;
        }
        double mu = it[k].g;
        if (mu <= EPS || used >= W - EPS) break;

        double step = it[k].w - it[k].x;
        int event = 0, joiner = -1;
        if (W - used < step) { step = W - used; event = 1; }
        if (mu / it[k].lam < step) { step = mu / it[k].lam; event = 2; }
        for (int i = 0; i < n; i++) {
            if (it[i].full || it[i].member || it[i].lam >= it[k].lam) continue;
            double gap = mu - it[i].g;
            if (gap < 0) gap = 0;
            double dj = gap / (it[k].lam - it[i].lam);
            if (dj < step) { step = dj; event = 3; joiner = i; }
        }

        it[k].x += step;
        used += step;
        for (int i = 0; i < n; i++)
            it[i].g -= fmin(it[i].lam, it[k].lam) * step;

        if (event == 0) { it[k].full = 1; it[k].x = it[k].w; it[k].member = 0; }
        else if (event == 3) it[joiner].member = 1;
        else break;
    }
    return used;
}

static void prt_ins(const Item *it, int n, double W) {
    printf("\nInput (n = %d, W = %.2f)\n", n, W);
    printf("%5s %10s %10s %10s %10s\n", "item", "value", "weight", "lambda", "density");
    for (int i = 0; i < n; i++)
        printf("%5d %10.2f %10.2f %10.4f %10.4f\n", i + 1, it[i].v, it[i].w, it[i].lam,
               it[i].v / it[i].w);
}

static double prt_sch(const Item *it, int n) {
    double *x = calloc((size_t)n, sizeof(double));
    Slot *s = malloc((size_t)n * sizeof(Slot));
    for (int i = 0; i < n; i++) x[i] = it[i].x;
    bld_slt(it, n, x, s);

    printf("\nOptimal schedule (decreasing decay rate)\n");
    printf("%5s %9s %9s %10s %12s %12s %12s\n", "item", "start t", "taken", "fraction",
           "density@start", "density@end", "value");
    double t = 0, total = 0;
    for (int i = 0; i < n; i++) {
        if (s[i].x <= EPS) continue;
        double gain = s[i].d * s[i].x - s[i].lam * (t * s[i].x + 0.5 * s[i].x * s[i].x);
        printf("%5d %9.3f %9.3f %9.1f%% %12.4f %12.4f %12.4f\n", s[i].id + 1, t, s[i].x,
               100.0 * s[i].x / it[s[i].id].w, s[i].d - s[i].lam * t,
               s[i].d - s[i].lam * (t + s[i].x), gain);
        total += gain;
        t += s[i].x;
    }
    printf("\nTotal time used : %.4f\n", t);
    printf("Maximum value   : %.6f\n", total);
    free(x);
    free(s);
    return total;
}

static int val_sol(Item *it, int n, double W, double used) {
    double *x = malloc((size_t)n * sizeof(double));
    double *y = malloc((size_t)n * sizeof(double));
    for (int i = 0; i < n; i++) x[i] = it[i].x;
    double base = sch_val(it, n, x);
    int bad = 0, trials = 4000;

    for (int t = 0; t < trials; t++) {
        for (int i = 0; i < n; i++) y[i] = x[i];
        int i = rand() % n, j = rand() % n;
        double shrink = pow(rnd_rng(0, 1), 3.0);
        int op = rand() % 3;
        if (op == 0 && i != j) {
            double amt = fmin(y[j], it[i].w - y[i]) * shrink;
            y[j] -= amt; y[i] += amt;
        } else if (op == 1) {
            double amt = fmin(it[i].w - y[i], W - used) * shrink;
            y[i] += amt;
        } else {
            y[j] -= y[j] * shrink;
        }
        if (sch_val(it, n, y) > base + 1e-7 * (1.0 + fabs(base))) bad++;
    }
    printf("\nValidation: %d random feasible perturbations tried, %d improved the value -> %s\n",
           trials, bad, bad == 0 ? "PASSED (solution is optimal)" : "FAILED");
    free(x);
    free(y);
    return bad == 0;
}

int main(void) {
    static Item items[MAX_N] = {
        {100.0, 5.0, 0.20, 0, 0, 0, 0, 0},
        {120.0, 8.0, 0.15, 0, 0, 0, 0, 0},
        {90.0,  6.0, 0.30, 0, 0, 0, 0, 0},
        {150.0, 10.0, 0.10, 0, 0, 0, 0, 0}
    };
    int n = 4;
    double W = 15.0;
    srand(42);

    printf("=== Q01: Fractional Knapsack with Deterioration Rate ===\n");
    prt_ins(items, n, W);
    double used = slv_knp(items, n, W);
    printf("Total weight used : %.4f\n", used);
    prt_sch(items, n);
    val_sol(items, n, W, used);
    return 0;
}
