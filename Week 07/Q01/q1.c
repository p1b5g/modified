#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    int x;
    int y;
} Point;

static int cmp_pt(const void *a, const void *b) {
    const Point *p = a, *q = b;
    if (p->x != q->x) return (p->x < q->x) ? -1 : 1;
    if (p->y != q->y) return (p->y < q->y) ? -1 : 1;
    return 0;
}

static int pt_ex(const Point *arr, int n, Point key) {
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = (lo + hi) >> 1;
        const Point *p = &arr[mid];
        if (p->x == key.x && p->y == key.y) return 1;
        if (p->x < key.x || (p->x == key.x && p->y < key.y)) lo = mid + 1;
        else hi = mid - 1;
    }
    return 0;
}

static int slv_tri(int n) {
    if (n <= 0) {
        printf("n must be positive.\n");
        return 1;
    }

    int T = n * (n + 1) / 2;
    Point *pts = malloc(sizeof(Point) * T);
    if (!pts) {
        perror("malloc");
        return 1;
    }

    int idx = 0;
    long long sumx = 0, sumy = 0;
    for (int r = 0; r < n; ++r) {
        for (int c = 0; c <= r; ++c) {
            pts[idx].x = 2 * c - r;
            pts[idx].y = r;
            sumx += pts[idx].x;
            sumy += pts[idx].y;
            ++idx;
        }
    }

    double Cx = (double)sumx / (double)T;
    double Cy = (double)sumy / (double)T;

    qsort(pts, T, sizeof(Point), cmp_pt);

    int ovlp = 0;
    for (int i = 0; i < T; ++i) {
        double rx = 2.0 * Cx - (double)pts[i].x;
        double ry = 2.0 * Cy - (double)pts[i].y;
        int ix = (int)llround(rx);
        int iy = (int)llround(ry);
        Point q = { ix, iy };
        if (pt_ex(pts, T, q)) ovlp++;
    }

    int min_mv = T - ovlp;
    printf("n = %d (rows)\n", n);
    printf("Total coins T = %d\n", T);
    printf("Coins that remain in place after 180deg rotation (overlap) = %d\n", ovlp);
    printf("Minimum moves (T - overlap) = %d\n", min_mv);

    free(pts);
    return 0;
}

int main(void) {
    int n = 5;
    return slv_tri(n);
}
