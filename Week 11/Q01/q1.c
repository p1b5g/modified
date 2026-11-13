#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF LLONG_MAX

typedef struct Edge {
    int to;
    int weight;
    struct Edge *next;
} Edge;

typedef struct {
    int num_v;
    int num_e;
    int src;
    long long pen_c;
    Edge **head;
} Graph;

typedef struct {
    int vtx;
    int parity;
} State;

typedef struct {
    State *data;
    int front;
    int back;
    int cap;
} Deque;

Graph *crt_grp(int v, int e, int src, long long pen_c) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (!g) {
        perror("Failed to allocate Graph");
        exit(EXIT_FAILURE);
    }
    g->num_v = v;
    g->num_e = e;
    g->src = src;
    g->pen_c = pen_c;
    g->head = (Edge **)calloc(v + 1, sizeof(Edge *));
    if (!g->head) {
        perror("Failed to allocate adjacency list");
        free(g);
        exit(EXIT_FAILURE);
    }
    return g;
}

void add_edg(Graph *g, int u, int v, int w) {
    Edge *new_e = (Edge *)malloc(sizeof(Edge));
    if (!new_e) {
        perror("Failed to allocate Edge");
        exit(EXIT_FAILURE);
    }
    new_e->to = v;
    new_e->weight = w;
    new_e->next = g->head[u];
    g->head[u] = new_e;
}

void fre_grp(Graph *g) {
    if (!g) return;
    for (int i = 1; i <= g->num_v; i++) {
        Edge *curr = g->head[i];
        while (curr) {
            Edge *temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(g->head);
    free(g);
}

void prt_grp(const Graph *g) {
    printf("\nGraph Summary: \n");
    printf("Vertices (|V|): %d | Directed Edges (|E|): %d | Source (S): %d | Odd Penalty (C): %lld\n",
           g->num_v, g->num_e, g->src, g->pen_c);
    printf("Adjacency List (u -> (v, weight)):\n");
    for (int u = 1; u <= g->num_v; u++) {
        printf("  Vertex %2d: ", u);
        Edge *curr = g->head[u];
        if (!curr) {
            printf("(no outgoing edges)");
        }
        while (curr) {
            printf("-> (%d, w=%d) ", curr->to, curr->weight);
            curr = curr->next;
        }
        printf("\n");
    }
}

Deque *crt_deq(int max_el) {
    Deque *dq = (Deque *)malloc(sizeof(Deque));
    if (!dq) {
        perror("Failed to allocate Deque");
        exit(EXIT_FAILURE);
    }
    dq->cap = 2 * max_el + 5;
    dq->data = (State *)malloc(dq->cap * sizeof(State));
    if (!dq->data) {
        perror("Failed to allocate Deque buffer");
        free(dq);
        exit(EXIT_FAILURE);
    }
    dq->front = max_el + 2;
    dq->back = max_el + 2;
    return dq;
}

bool deq_emp(const Deque *dq) {
    return dq->front == dq->back;
}

void psh_frt(Deque *dq, int vtx, int parity) {
    dq->front--;
    dq->data[dq->front].vtx = vtx;
    dq->data[dq->front].parity = parity;
}

void psh_bck(Deque *dq, int vtx, int parity) {
    dq->data[dq->back].vtx = vtx;
    dq->data[dq->back].parity = parity;
    dq->back++;
}

State pop_frt(Deque *dq) {
    State s = dq->data[dq->front];
    dq->front++;
    return s;
}

void fre_deq(Deque *dq) {
    if (!dq) return;
    free(dq->data);
    free(dq);
}

void slv_bfs(const Graph *g) {
    int n = g->num_v;

    long long (*dist)[2] = malloc((n + 1) * sizeof(*dist));
    bool (*vis)[2] = calloc(n + 1, sizeof(*vis));
    if (!dist || !vis) {
        perror("Failed to allocate 0-1 BFS tables");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i <= n; i++) {
        dist[i][0] = INF;
        dist[i][1] = INF;
    }

    int max_psh = 2 * (n + g->num_e) + 10;
    Deque *dq = crt_deq(max_psh);

    dist[g->src][0] = 0;
    psh_frt(dq, g->src, 0);

    while (!deq_emp(dq)) {
        State curr = pop_frt(dq);
        int u = curr.vtx;
        int p = curr.parity;

        if (vis[u][p]) {
            continue;
        }
        vis[u][p] = true;

        long long d = dist[u][p];

        for (Edge *e = g->head[u]; e != NULL; e = e->next) {
            int v = e->to;
            int w = e->weight;
            int nxt_par = p ^ w;
            long long nxt_dst = d + w;

            if (nxt_dst < dist[v][nxt_par]) {
                dist[v][nxt_par] = nxt_dst;
                if (w == 0) {
                    psh_frt(dq, v, nxt_par);
                } else {
                    psh_bck(dq, v, nxt_par);
                }
            }
        }
    }

    printf("\n--- Minimum Effective Cost from Source S = %d (Penalty C = %lld) ---\n",
           g->src, g->pen_c);
    printf("%-10s | %-18s | %-18s | %-20s\n",
           "Vertex (v)", "Even k1 W(P)", "Odd k1 W(P)+C", "Min Effective Cost");
    printf("--------------------------------------------------------------------------\n");

    for (int v = 1; v <= n; v++) {
        long long evn_cst = dist[v][0];
        long long odd_cst = (dist[v][1] == INF) ? INF : (dist[v][1] + g->pen_c);
        long long bst_cst = (evn_cst < odd_cst) ? evn_cst : odd_cst;

        char evn_str[80], odd_str[80], bst_str[80];
        if (evn_cst == INF) snprintf(evn_str, sizeof(evn_str), "INF");
        else snprintf(evn_str, sizeof(evn_str), "%lld", evn_cst);

        if (odd_cst == INF) snprintf(odd_str, sizeof(odd_str), "INF");
        else snprintf(odd_str, sizeof(odd_str), "%lld (%lld+%lld)", odd_cst, dist[v][1], g->pen_c);

        if (bst_cst == INF) snprintf(bst_str, sizeof(bst_str), "UNREACHABLE");
        else snprintf(bst_str, sizeof(bst_str), "%lld", bst_cst);

        printf("%-10d | %-18s | %-18s | %-20s\n", v, evn_str, odd_str, bst_str);
    }

    fre_deq(dq);
    free(vis);
    free(dist);
}

int main(void) {
    printf("=================================================================\n");
    printf("  Q01: Shortest Path on Parity-Constrained Graphs via 0-1 BFS\n");
    printf("=================================================================\n");

    Graph *g = crt_grp(5, 6, 1, 3);
    add_edg(g, 1, 2, 1);
    add_edg(g, 2, 3, 0);
    add_edg(g, 3, 4, 1);
    add_edg(g, 1, 4, 1);
    add_edg(g, 4, 5, 0);
    add_edg(g, 2, 5, 1);

    prt_grp(g);
    slv_bfs(g);
    fre_grp(g);

    return EXIT_SUCCESS;
}
