#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

typedef struct Node {
    int vtx;
    struct Node *next;
} Node;

typedef struct {
    int num_v;
    int num_e;
    int num_h;
    int rad;
    int *hubs;
    Node **adj;
} VGraph;

VGraph *crt_grp(int v, int e, int k, int r) {
    VGraph *g = (VGraph *)malloc(sizeof(VGraph));
    if (!g) {
        perror("Failed to allocate VGraph");
        exit(EXIT_FAILURE);
    }
    g->num_v = v;
    g->num_e = e;
    g->num_h = k;
    g->rad = r;
    g->hubs = (int *)calloc(k + 1, sizeof(int));
    g->adj = (Node **)calloc(v + 1, sizeof(Node *));
    if (!g->hubs || !g->adj) {
        perror("Failed to allocate graph arrays");
        exit(EXIT_FAILURE);
    }
    return g;
}

void add_edg(VGraph *g, int u, int v) {
    Node *n1 = (Node *)malloc(sizeof(Node));
    Node *n2 = (Node *)malloc(sizeof(Node));
    if (!n1 || !n2) {
        perror("Failed to allocate adjacency node");
        exit(EXIT_FAILURE);
    }
    n1->vtx = v;
    n1->next = g->adj[u];
    g->adj[u] = n1;

    n2->vtx = u;
    n2->next = g->adj[v];
    g->adj[v] = n2;
}

void fre_grp(VGraph *g) {
    if (!g) return;
    for (int i = 1; i <= g->num_v; i++) {
        Node *curr = g->adj[i];
        while (curr) {
            Node *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(g->adj);
    free(g->hubs);
    free(g);
}

void prt_sum(const VGraph *g) {
    printf("\n--- Graph & Hubs Summary ---\n");
    printf("Vertices (|V|): %d | Undirected Edges (|E|): %d | Hubs (K): %d | Radius (R): %d\n",
           g->num_v, g->num_e, g->num_h, g->rad);
    printf("Designated Hubs H = { ");
    for (int i = 1; i <= g->num_h; i++) {
        printf("h_%d=%d%s", i, g->hubs[i], (i < g->num_h) ? ", " : " ");
    }
    printf("}\n");

    printf("Adjacency List:\n");
    for (int u = 1; u <= g->num_v; u++) {
        printf("  Vertex %2d: ", u);
        Node *curr = g->adj[u];
        if (!curr) printf("(isolated)");
        while (curr) {
            printf("%d ", curr->vtx);
            curr = curr->next;
        }
        printf("\n");
    }
}

void slv_bfs(const VGraph *g) {
    int n = g->num_v;
    int *dist = (int *)malloc((n + 1) * sizeof(int));
    int *owner = (int *)malloc((n + 1) * sizeof(int));
    int *queue = (int *)malloc((n + 1) * sizeof(int));

    if (!dist || !owner || !queue) {
        perror("Failed to allocate BFS arrays");
        exit(EXIT_FAILURE);
    }

    for (int v = 1; v <= n; v++) {
        dist[v] = INF;
        owner[v] = -1;
    }

    int front = 0, back = 0;

    for (int i = 1; i <= g->num_h; i++) {
        int h = g->hubs[i];
        if (dist[h] == INF) {
            dist[h] = 0;
            owner[h] = i;
            queue[back++] = h;
        }
    }

    while (front < back) {
        int u = queue[front++];

        if (dist[u] >= g->rad) {
            continue;
        }

        for (Node *curr = g->adj[u]; curr != NULL; curr = curr->next) {
            int v = curr->vtx;
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                owner[v] = owner[u];
                queue[back++] = v;
            } else if (dist[v] == dist[u] + 1 && owner[u] < owner[v]) {
                owner[v] = owner[u];
            }
        }
    }

    printf("\n--- Voronoi Cell Partitioning Results (Radius R = %d) ---\n", g->rad);
    printf("%-12s | %-18s | %-16s | %-20s\n",
           "Vertex (v)", "owner[v] (Hub ID)", "Hub Vertex (h_i)", "Shortest Dist d_min");
    printf("---------------------------------------------------------------------------\n");

    for (int v = 1; v <= n; v++) {
        if (owner[v] == -1) {
            printf("%-12d | %-18d | %-16s | %-20s\n",
                   v, -1, "N/A", "OUTSIDE (> R / INF)");
        } else {
            printf("%-12d | %-18d | %-16d | %-20d\n",
                   v, owner[v], g->hubs[owner[v]], dist[v]);
        }
    }

    free(dist);
    free(owner);
    free(queue);
}

int main(void) {
    printf("=========================================================================\n");
    printf("  Q02: Multi-Source BFS for Radius-Constrained Voronoi Cell Partitioning\n");
    printf("=========================================================================\n");

    VGraph *g = crt_grp(7, 7, 2, 2);
    g->hubs[1] = 1;
    g->hubs[2] = 6;

    add_edg(g, 1, 2);
    add_edg(g, 2, 3);
    add_edg(g, 3, 4);
    add_edg(g, 4, 5);
    add_edg(g, 5, 6);
    add_edg(g, 2, 5);
    add_edg(g, 6, 7);

    prt_sum(g);
    slv_bfs(g);
    fre_grp(g);

    return EXIT_SUCCESS;
}
