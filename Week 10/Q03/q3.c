#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct {
    int u;
    int v;
    long long edge_w;
} WEdge;

typedef struct {
    int *par;
    int *rank;
    int *cmp_sz;
    long long *cmp_vw;
    long long *cmp_ew;
    int num_v;
} WgtDSU;

WgtDSU *crt_dsu(int V, const long long *v_wgts) {
    WgtDSU *dsu = (WgtDSU *)malloc(sizeof(WgtDSU));
    dsu->num_v = V;
    dsu->par = (int *)malloc((V + 1) * sizeof(int));
    dsu->rank = (int *)calloc((V + 1), sizeof(int));
    dsu->cmp_sz = (int *)malloc((V + 1) * sizeof(int));
    dsu->cmp_vw = (long long *)malloc((V + 1) * sizeof(long long));
    dsu->cmp_ew = (long long *)calloc((V + 1), sizeof(long long));

    for (int i = 1; i <= V; i++) {
        dsu->par[i] = i;
        dsu->cmp_sz[i] = 1;
        dsu->cmp_vw[i] = v_wgts[i];
    }
    return dsu;
}

void fre_dsu(WgtDSU *dsu) {
    if (dsu) {
        free(dsu->par);
        free(dsu->rank);
        free(dsu->cmp_sz);
        free(dsu->cmp_vw);
        free(dsu->cmp_ew);
        free(dsu);
    }
}

int fnd_set(WgtDSU *dsu, int v) {
    if (dsu->par[v] != v) {
        dsu->par[v] = fnd_set(dsu, dsu->par[v]);
    }
    return dsu->par[v];
}

void uni_cmp(WgtDSU *dsu, int u, int v, long long edge_w) {
    int root_u = fnd_set(dsu, u);
    int root_v = fnd_set(dsu, v);

    if (root_u == root_v) {
        dsu->cmp_ew[root_u] += edge_w;
        return;
    }

    if (dsu->rank[root_u] < dsu->rank[root_v]) {
        dsu->par[root_u] = root_v;
        dsu->cmp_sz[root_v] += dsu->cmp_sz[root_u];
        dsu->cmp_vw[root_v] += dsu->cmp_vw[root_u];
        dsu->cmp_ew[root_v] += dsu->cmp_ew[root_u] + edge_w;
    } else if (dsu->rank[root_u] > dsu->rank[root_v]) {
        dsu->par[root_v] = root_u;
        dsu->cmp_sz[root_u] += dsu->cmp_sz[root_v];
        dsu->cmp_vw[root_u] += dsu->cmp_vw[root_v];
        dsu->cmp_ew[root_u] += dsu->cmp_ew[root_v] + edge_w;
    } else {
        dsu->par[root_v] = root_u;
        dsu->cmp_sz[root_u] += dsu->cmp_sz[root_v];
        dsu->cmp_vw[root_u] += dsu->cmp_vw[root_v];
        dsu->cmp_ew[root_u] += dsu->cmp_ew[root_v] + edge_w;
        dsu->rank[root_u]++;
    }
}

long long qry_max(WgtDSU *dsu, int K, int *bst_rt, int *bst_sz) {
    long long max_w = LLONG_MIN;
    *bst_rt = -1;
    *bst_sz = 0;

    for (int i = 1; i <= dsu->num_v; i++) {
        if (dsu->par[i] == i && dsu->cmp_sz[i] <= K) {
            if (dsu->cmp_vw[i] > max_w) {
                max_w = dsu->cmp_vw[i];
                *bst_rt = i;
                *bst_sz = dsu->cmp_sz[i];
            }
        }
    }
    return max_w;
}

void prc_edg(int V, int E, int K, const long long *v_wgts, const WEdge *edges) {
    WgtDSU *dsu = crt_dsu(V, v_wgts);
    int bst_rt = -1, bst_sz = 0;

    printf("\n--- Maximum Weight Component (Size <= %d) Log ---\n", K);
    long long ini_max = qry_max(dsu, K, &bst_rt, &bst_sz);
    if (ini_max != LLONG_MIN) {
        printf("Initial State (0 edges) -> Max Total Vertex Weight = %lld (Root = %d, Size = %d)\n",
               ini_max, bst_rt, bst_sz);
    } else {
        printf("Initial State (0 edges) -> No component has size <= %d\n", K);
    }

    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        long long w = edges[i].edge_w;

        if (u < 1 || u > V || v < 1 || v > V) {
            printf("Step %2d: Edge (%d, %d, w=%lld) -> INVALID VERTEX ID (Skipped)\n", i + 1, u, v, w);
            continue;
        }

        uni_cmp(dsu, u, v, w);
        long long cur_max = qry_max(dsu, K, &bst_rt, &bst_sz);

        if (cur_max != LLONG_MIN) {
            printf("Step %2d: Added Edge (%d, %d, w=%lld) -> Max Valid Component Vertex Weight = %lld (Root = %d, Size = %d)\n",
                   i + 1, u, v, w, cur_max, bst_rt, bst_sz);
        } else {
            printf("Step %2d: Added Edge (%d, %d, w=%lld) -> No component has size <= %d\n",
                   i + 1, u, v, w, K);
        }
    }

    fre_dsu(dsu);
}

int main(void) {
    int V = 6, K = 3;
    long long v_wgts[] = {0, 10, 20, 15, 25, 30, 12};
    WEdge edges[] = {
        {1, 2, 5},
        {3, 4, 8},
        {5, 6, 4},
        {2, 3, 6},
        {1, 5, 7}
    };
    int E = sizeof(edges) / sizeof(edges[0]);

    printf("=============================================================\n");
    printf(" Q03: Maximum Weight Component with Size Constraints (<= K)\n");
    printf("=============================================================\n");

    prc_edg(V, E, K, v_wgts, edges);
    return EXIT_SUCCESS;
}
