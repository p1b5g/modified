#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int *head;
    int *to;
    int *next;
    int edg_cnt;
    int num_v;
} Graph;

typedef struct {
    int *par;
    int *rank;
    long long *cmp_wgt;
    int num_v;
} RevDSU;

Graph *crt_grp(int V, int max_edg) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->num_v = V;
    g->edg_cnt = 0;
    g->head = (int *)malloc((V + 1) * sizeof(int));
    g->to = (int *)malloc((2 * max_edg + 2) * sizeof(int));
    g->next = (int *)malloc((2 * max_edg + 2) * sizeof(int));

    for (int i = 1; i <= V; i++) {
        g->head[i] = -1;
    }
    return g;
}

void add_edg(Graph *g, int u, int v) {
    g->to[g->edg_cnt] = v;
    g->next[g->edg_cnt] = g->head[u];
    g->head[u] = g->edg_cnt++;

    g->to[g->edg_cnt] = u;
    g->next[g->edg_cnt] = g->head[v];
    g->head[v] = g->edg_cnt++;
}

void fre_grp(Graph *g) {
    if (g) {
        free(g->head);
        free(g->to);
        free(g->next);
        free(g);
    }
}

RevDSU *crt_dsu(int V, const long long *weights) {
    RevDSU *dsu = (RevDSU *)malloc(sizeof(RevDSU));
    dsu->num_v = V;
    dsu->par = (int *)malloc((V + 1) * sizeof(int));
    dsu->rank = (int *)calloc((V + 1), sizeof(int));
    dsu->cmp_wgt = (long long *)malloc((V + 1) * sizeof(long long));

    for (int i = 1; i <= V; i++) {
        dsu->par[i] = i;
        dsu->cmp_wgt[i] = weights[i];
    }
    return dsu;
}

void fre_dsu(RevDSU *dsu) {
    if (dsu) {
        free(dsu->par);
        free(dsu->rank);
        free(dsu->cmp_wgt);
        free(dsu);
    }
}

int fnd_set(RevDSU *dsu, int v) {
    if (dsu->par[v] != v) {
        dsu->par[v] = fnd_set(dsu, dsu->par[v]);
    }
    return dsu->par[v];
}

long long uni_set(RevDSU *dsu, int u, int v) {
    int root_u = fnd_set(dsu, u);
    int root_v = fnd_set(dsu, v);

    if (root_u == root_v) {
        return dsu->cmp_wgt[root_u];
    }

    if (dsu->rank[root_u] < dsu->rank[root_v]) {
        dsu->par[root_u] = root_v;
        dsu->cmp_wgt[root_v] += dsu->cmp_wgt[root_u];
        return dsu->cmp_wgt[root_v];
    } else if (dsu->rank[root_u] > dsu->rank[root_v]) {
        dsu->par[root_v] = root_u;
        dsu->cmp_wgt[root_u] += dsu->cmp_wgt[root_v];
        return dsu->cmp_wgt[root_u];
    } else {
        dsu->par[root_v] = root_u;
        dsu->cmp_wgt[root_u] += dsu->cmp_wgt[root_v];
        dsu->rank[root_u]++;
        return dsu->cmp_wgt[root_u];
    }
}

void prc_del(int V, const long long *weights, Graph *g, const int *del_ord) {
    RevDSU *dsu = crt_dsu(V, weights);
    bool *active = (bool *)calloc(V + 1, sizeof(bool));
    long long *ans_del = (long long *)calloc(V + 1, sizeof(long long));

    long long cur_max = 0;
    ans_del[V] = 0;

    for (int step = V; step >= 1; step--) {
        ans_del[step] = cur_max;

        int u = del_ord[step];
        active[u] = true;

        long long u_wgt = dsu->cmp_wgt[fnd_set(dsu, u)];
        if (u_wgt > cur_max) {
            cur_max = u_wgt;
        }

        for (int e = g->head[u]; e != -1; e = g->next[e]) {
            int v = g->to[e];
            if (active[v]) {
                long long mrg_wgt = uni_set(dsu, u, v);
                if (mrg_wgt > cur_max) {
                    cur_max = mrg_wgt;
                }
            }
        }
    }

    printf("\n--- Reverse Time-Step DSU Vertex Deletion Log ---\n");
    printf("Initial Graph (before any deletion) -> Max Component Vertex Sum = %lld\n", cur_max);
    for (int step = 1; step <= V; step++) {
        printf("After deleting vertex %2d (Step %2d/%2d) -> Max Remaining Component Sum = %lld\n",
               del_ord[step], step, V, ans_del[step]);
    }

    free(active);
    free(ans_del);
    fre_dsu(dsu);
}

int main(void) {
    int V = 5, E = 5;
    long long weights[] = {0, 10, 20, 30, 40, 50};
    int del_ord[] = {0, 3, 1, 5, 2, 4};

    Graph *g = crt_grp(V, E);
    add_edg(g, 1, 2);
    add_edg(g, 2, 3);
    add_edg(g, 3, 4);
    add_edg(g, 4, 5);
    add_edg(g, 1, 5);

    printf("=============================================================\n");
    printf(" Q04: Reverse Time-Step DSU Application (Vertex Deletions)\n");
    printf("=============================================================\n");

    prc_del(V, weights, g, del_ord);

    fre_grp(g);
    return EXIT_SUCCESS;
}
