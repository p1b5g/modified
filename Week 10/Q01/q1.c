#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int u;
    int v;
} Edge;

typedef struct {
    int *par;
    int *rank;
    int *parity;
    int num_v;
} BipDSU;

typedef struct {
    int root;
    int p_root;
} FndRes;

BipDSU *crt_dsu(int V) {
    BipDSU *dsu = (BipDSU *)malloc(sizeof(BipDSU));
    dsu->num_v = V;
    dsu->par = (int *)malloc((V + 1) * sizeof(int));
    dsu->rank = (int *)calloc((V + 1), sizeof(int));
    dsu->parity = (int *)calloc((V + 1), sizeof(int));

    for (int i = 1; i <= V; i++) {
        dsu->par[i] = i;
    }
    return dsu;
}

void fre_dsu(BipDSU *dsu) {
    if (dsu) {
        free(dsu->par);
        free(dsu->rank);
        free(dsu->parity);
        free(dsu);
    }
}

FndRes fnd_set(BipDSU *dsu, int v) {
    if (v == dsu->par[v]) {
        FndRes res = {v, 0};
        return res;
    }
    FndRes par_res = fnd_set(dsu, dsu->par[v]);
    dsu->par[v] = par_res.root;
    dsu->parity[v] ^= par_res.p_root;

    FndRes res = {dsu->par[v], dsu->parity[v]};
    return res;
}

bool add_edg(BipDSU *dsu, int u, int v) {
    if (u == v) {
        return false;
    }

    FndRes res_u = fnd_set(dsu, u);
    FndRes res_v = fnd_set(dsu, v);

    if (res_u.root == res_v.root) {
        if (res_u.p_root == res_v.p_root) {
            return false;
        }
        return true;
    }

    int root_u = res_u.root;
    int root_v = res_v.root;

    if (dsu->rank[root_u] < dsu->rank[root_v]) {
        dsu->par[root_u] = root_v;
        dsu->parity[root_u] = res_u.p_root ^ res_v.p_root ^ 1;
    } else if (dsu->rank[root_u] > dsu->rank[root_v]) {
        dsu->par[root_v] = root_u;
        dsu->parity[root_v] = res_u.p_root ^ res_v.p_root ^ 1;
    } else {
        dsu->par[root_v] = root_u;
        dsu->parity[root_v] = res_u.p_root ^ res_v.p_root ^ 1;
        dsu->rank[root_u]++;
    }
    return true;
}

void prc_edg(int V, int E, Edge *edges) {
    BipDSU *dsu = crt_dsu(V);
    int acc_cnt = 0;
    int skp_cnt = 0;

    printf("\n--- Online Graph-Bipartiteness Testing Log ---\n");
    for (int i = 0; i < E; i++) {
        int u = edges[i].u;
        int v = edges[i].v;

        if (u < 1 || u > V || v < 1 || v > V) {
            printf("Step %2d: Edge (%d, %d) -> INVALID VERTEX ID (Skipped)\n", i + 1, u, v);
            skp_cnt++;
            continue;
        }

        if (add_edg(dsu, u, v)) {
            acc_cnt++;
            printf("Step %2d: Edge (%d, %d) -> ADDED   | Graph remains bipartite.\n", i + 1, u, v);
        } else {
            skp_cnt++;
            printf("Step %2d: Edge (%d, %d) -> SKIPPED | Violates bipartiteness (introduces an odd cycle).\n", i + 1, u, v);
        }
    }

    printf("\nSummary: %d edge(s) added, %d edge(s) skipped. Final graph is bipartite.\n",
           acc_cnt, skp_cnt);

    fre_dsu(dsu);
}

int main(void) {
    int V = 5;
    Edge edges[] = {{1, 2}, {2, 3}, {3, 4}, {4, 1}, {1, 3}, {4, 5}};
    int E = sizeof(edges) / sizeof(edges[0]);

    printf("=============================================================\n");
    printf(" Q01: Online Graph-Bipartiteness Testing (Parity-Based DSU)\n");
    printf("=============================================================\n");

    prc_edg(V, E, edges);
    return EXIT_SUCCESS;
}
