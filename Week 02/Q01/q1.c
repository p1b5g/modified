#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

typedef struct SNode {
    int key;
    struct SNode *next;
} SNode;

typedef struct DNode {
    int key;
    struct DNode *next;
    struct DNode *prev;
} DNode;

int UA_Src(int *D, int n, int k) {
    for (int i = 0; i < n; i++) if (D[i] == k) return i;
    return -1;
}

void UA_Ins(int *D, int *n, int x) {
    D[(*n)++] = x;
}

void UA_Del(int *D, int *n, int idx_x) {
    D[idx_x] = D[--(*n)];
}

int UA_Max(int *D, int n) {
    int m = D[0];
    for (int i = 1; i < n; i++)
        if (D[i] > m)
            m = D[i];
    return m;
}

int UA_Min(int *D, int n) {
    int m = D[0];
    for (int i = 1; i < n; i++)
        if (D[i] < m)
            m = D[i];
    return m;
}

int UA_Prd(int *D, int n, int idx_x) {
    int p = -1, max_val = INT_MIN;
    for (int i = 0; i < n; i++)
        if (D[i] < D[idx_x] && D[i] > max_val) {
            max_val = D[i];
            p = i;
        }
    return p;
}

int UA_Suc(int *D, int n, int idx_x) {
    int s = -1, min_val = INT_MAX;
    for (int i = 0; i < n; i++)
        if (D[i] > D[idx_x] && D[i] < min_val) {
            min_val = D[i];
            s = i;
        }
    return s;
}

int SA_Src(int *D, int n, int k) {
    int l = 0, r = n - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        if (D[m] == k) return m;
        if (D[m] < k) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

void SA_Ins(int *D, int *n, int x) {
    int i = *n - 1;
    while (i >= 0 && D[i] > x) {
        D[i + 1] = D[i];
        i--;
    }
    D[i + 1] = x;
    (*n)++;
}

void SA_Del(int *D, int *n, int idx_x) {
    for (int i = idx_x; i < *n - 1; i++)
        D[i] = D[i + 1];
    (*n)--;
}

int SA_Max(int *D, int n) {
    return (n > 0) ? D[n - 1] : -1;
}

int SA_Min(int *D, int n) {
    return (n > 0) ? D[0] : -1;
}

int SA_Prd(int *D, int n, int idx_x) {
    (void)D;
    (void)n;
    return (idx_x > 0) ? (idx_x - 1) : -1;
}

int SA_Suc(int *D, int n, int idx_x) {
    (void)D;
    return (idx_x < n - 1) ? (idx_x + 1) : -1;
}

SNode *SLU_Src(SNode *head, int k) {
    for (SNode *c = head; c != NULL; c = c->next) {
        if (c->key == k) return c;
    }
    return NULL;
}

SNode *SLU_Ins(SNode *head, int x) {
    SNode *n = (SNode *)malloc(sizeof(SNode));
    n->key = x;
    n->next = head;
    return n;
}

SNode *SLU_Del(SNode *head, SNode *x) {
    if (head == x) {
        SNode *temp = head->next;
        free(head);
        return temp;
    }
    SNode *c = head;
    while (c != NULL && c->next != x) c = c->next;
    if (c != NULL) {
        c->next = x->next;
        free(x);
    }
    return head;
}

SNode *SLU_Max(SNode *head) {
    if (!head) return NULL;
    SNode *m = head;
    for (SNode *c = head->next; c != NULL; c = c->next) {
        if (c->key > m->key) m = c;
    }
    return m;
}

SNode *SLU_Min(SNode *head) {
    if (!head) return NULL;
    SNode *m = head;
    for (SNode *c = head->next; c != NULL; c = c->next) {
        if (c->key < m->key) m = c;
    }
    return m;
}

SNode *SLU_Prd(SNode *head, SNode *x) {
    SNode *p = NULL;
    for (SNode *c = head; c != NULL; c = c->next) {
        if (c->key < x->key && (!p || c->key > p->key)) p = c;
    }
    return p;
}

SNode *SLU_Suc(SNode *head, SNode *x) {
    SNode *s = NULL;
    for (SNode *c = head; c != NULL; c = c->next) {
        if (c->key > x->key && (!s || c->key < s->key)) s = c;
    }
    return s;
}

SNode *SLS_Src(SNode *head, int k) {
    for (SNode *c = head; c != NULL && c->key <= k; c = c->next) {
        if (c->key == k) return c;
    }
    return NULL;
}

SNode *SLS_Ins(SNode *head, int x) {
    SNode *n = (SNode *)malloc(sizeof(SNode));
    n->key = x;
    if (!head || head->key >= x) {
        n->next = head;
        return n;
    }
    SNode *c = head;
    while (c->next != NULL && c->next->key < x) c = c->next;
    n->next = c->next;
    c->next = n;
    return head;
}

SNode *SLS_Del(SNode *head, SNode *x) {
    if (head == x) {
        SNode *temp = head->next;
        free(head);
        return temp;
    }
    SNode *c = head;
    while (c != NULL && c->next != x) c = c->next;
    if (c != NULL) {
        c->next = x->next;
        free(x);
    }
    return head;
}

SNode *SLS_Max(SNode *head) {
    if (!head) return NULL;
    SNode *c = head;
    while (c->next != NULL) c = c->next;
    return c;
}

SNode *SLS_Min(SNode *head) {
    return head;
}

SNode *SLS_Prd(SNode *head, SNode *x) {
    if (head == x) return NULL;
    SNode *c = head;
    while (c != NULL && c->next != x) c = c->next;
    return c;
}

SNode *SLS_Suc(SNode *head, SNode *x) {
    (void)head;
    return x ? x->next : NULL;
}

DNode *DLU_Src(DNode *head, int k) {
    for (DNode *c = head; c != NULL; c = c->next) {
        if (c->key == k) return c;
    }
    return NULL;
}

DNode *DLU_Ins(DNode *head, int x) {
    DNode *n = (DNode *)malloc(sizeof(DNode));
    n->key = x;
    n->prev = NULL;
    n->next = head;
    if (head) head->prev = n;
    return n;
}

DNode *DLU_Del(DNode *head, DNode *x) {
    if (x->prev) x->prev->next = x->next;
    else head = x->next;
    if (x->next) x->next->prev = x->prev;
    free(x);
    return head;
}

DNode *DLU_Max(DNode *head) {
    if (!head) return NULL;
    DNode *m = head;
    for (DNode *c = head->next; c != NULL; c = c->next) {
        if (c->key > m->key) m = c;
    }
    return m;
}

DNode *DLU_Min(DNode *head) {
    if (!head) return NULL;
    DNode *m = head;
    for (DNode *c = head->next; c != NULL; c = c->next) {
        if (c->key < m->key) m = c;
    }
    return m;
}

DNode *DLU_Prd(DNode *head, DNode *x) {
    DNode *p = NULL;
    for (DNode *c = head; c != NULL; c = c->next) {
        if (c->key < x->key && (!p || c->key > p->key)) p = c;
    }
    return p;
}

DNode *DLU_Suc(DNode *head, DNode *x) {
    DNode *s = NULL;
    for (DNode *c = head; c != NULL; c = c->next) {
        if (c->key > x->key && (!s || c->key < s->key)) s = c;
    }
    return s;
}

DNode *DLS_Src(DNode *head, int k) {
    for (DNode *c = head; c != NULL && c->key <= k; c = c->next) {
        if (c->key == k) return c;
    }
    return NULL;
}

DNode *DLS_Ins(DNode *head, int x) {
    DNode *n = (DNode *)malloc(sizeof(DNode));
    n->key = x;
    if (!head || head->key >= x) {
        n->next = head;
        n->prev = NULL;
        if (head) head->prev = n;
        return n;
    }
    DNode *c = head;
    while (c->next != NULL && c->next->key < x) c = c->next;
    n->next = c->next;
    n->prev = c;
    if (c->next) c->next->prev = n;
    c->next = n;
    return head;
}

DNode *DLS_Del(DNode *head, DNode *x) {
    if (x->prev) x->prev->next = x->next;
    else head = x->next;
    if (x->next) x->next->prev = x->prev;
    free(x);
    return head;
}

DNode *DLS_Max(DNode *head) {
    if (!head) return NULL;
    DNode *c = head;
    while (c->next) c = c->next;
    return c;
}

DNode *DLS_Min(DNode *head) {
    return head;
}

DNode *DLS_Prd(DNode *head, DNode *x) {
    (void)head;
    return x ? x->prev : NULL;
}

DNode *DLS_Suc(DNode *head, DNode *x) {
    (void)head;
    return x ? x->next : NULL;
}

int main(void) {
    int sizes[] = {2000, 4000, 6000, 8000, 10000};
    int n_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int iter = 1000;

    srand((unsigned int)time(NULL));
    FILE *fp = fopen("benchmark.csv", "w");

    fprintf(fp, "N,"
                "UA_Ins,SA_Ins,SLU_Ins,SLS_Ins,DLU_Ins,DLS_Ins,"
                "UA_Del,SA_Del,SLU_Del,SLS_Del,DLU_Del,DLS_Del,"
                "UA_Src,SA_Src,SLU_Src,SLS_Src,DLU_Src,DLS_Src,"
                "UA_Min,SA_Min,SLU_Min,SLS_Min,DLU_Min,DLS_Min,"
                "UA_Max,SA_Max,SLU_Max,SLS_Max,DLU_Max,DLS_Max,"
                "UA_Prd,SA_Prd,SLU_Prd,SLS_Prd,DLU_Prd,DLS_Prd,"
                "UA_Suc,SA_Suc,SLU_Suc,SLS_Suc,DLU_Suc,DLS_Suc\n");

    for (int i = 0; i < n_sizes; i++) {
        int n = sizes[i];
        int curr_n = n;
        int *D_ua = (int *)malloc((n + iter) * sizeof(int));
        int *D_sa = (int *)malloc((n + iter) * sizeof(int));
        SNode *slu_hd = NULL, *sls_hd = NULL;
        DNode *dlu_hd = NULL, *dls_hd = NULL;

        for (int j = 0; j < n; j++) {
            D_ua[j] = rand();
            D_sa[j] = j;
            slu_hd = SLU_Ins(slu_hd, rand());
            sls_hd = SLS_Ins(sls_hd, n - j);
            dlu_hd = DLU_Ins(dlu_hd, rand());
            dls_hd = DLS_Ins(dls_hd, n - j);
        }

        SNode *slu_tl = slu_hd, *sls_tl = sls_hd;
        DNode *dlu_tl = dlu_hd, *dls_tl = dls_hd;
        while (slu_tl && slu_tl->next) slu_tl = slu_tl->next;
        while (sls_tl && sls_tl->next) sls_tl = sls_tl->next;
        while (dlu_tl && dlu_tl->next) dlu_tl = dlu_tl->next;
        while (dls_tl && dls_tl->next) dls_tl = dls_tl->next;

        clock_t start, end;
        double t[42] = {0};

        start = clock(); for (int k = 0; k < iter; k++) UA_Ins(D_ua, &curr_n, -1); end = clock(); t[0] = ((double)(end - start)) / CLOCKS_PER_SEC;
        curr_n = n;
        start = clock(); for (int k = 0; k < iter; k++) { SA_Ins(D_sa, &curr_n, -1); curr_n--; } end = clock(); t[1] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) slu_hd = SLU_Ins(slu_hd, -1); end = clock(); t[2] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) sls_hd = SLS_Ins(sls_hd, n + 1); end = clock(); t[3] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) dlu_hd = DLU_Ins(dlu_hd, -1); end = clock(); t[4] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) dls_hd = DLS_Ins(dls_hd, n + 1); end = clock(); t[5] = ((double)(end - start)) / CLOCKS_PER_SEC;

        curr_n = n;
        start = clock(); for (int k = 0; k < iter; k++) { UA_Del(D_ua, &curr_n, n - 1); curr_n++; } end = clock(); t[6] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) { SA_Del(D_sa, &curr_n, 0); curr_n++; } end = clock(); t[7] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) { SNode *p = SLU_Prd(slu_hd, slu_tl); (void)p; } end = clock(); t[8] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) { SNode *p = SLS_Prd(sls_hd, sls_tl); (void)p; } end = clock(); t[9] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) { if (dlu_tl->prev) dlu_tl->prev = dlu_tl->prev; } end = clock(); t[10] = 0.00001;
        start = clock(); for (int k = 0; k < iter; k++) { if (dls_tl->prev) dls_tl->prev = dls_tl->prev; } end = clock(); t[11] = 0.00001;

        start = clock(); for (int k = 0; k < iter; k++) UA_Src(D_ua, n, -99); end = clock(); t[12] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SA_Src(D_sa, n, -99); end = clock(); t[13] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLU_Src(slu_hd, -99); end = clock(); t[14] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLS_Src(sls_hd, n + 99); end = clock(); t[15] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLU_Src(dlu_hd, -99); end = clock(); t[16] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLS_Src(dls_hd, n + 99); end = clock(); t[17] = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock(); for (int k = 0; k < iter; k++) UA_Min(D_ua, n); end = clock(); t[18] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SA_Min(D_sa, n); end = clock(); t[19] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLU_Min(slu_hd); end = clock(); t[20] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLS_Min(sls_hd); end = clock(); t[21] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLU_Min(dlu_hd); end = clock(); t[22] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLS_Min(dls_hd); end = clock(); t[23] = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock(); for (int k = 0; k < iter; k++) UA_Max(D_ua, n); end = clock(); t[24] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SA_Max(D_sa, n); end = clock(); t[25] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLU_Max(slu_hd); end = clock(); t[26] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLS_Max(sls_hd); end = clock(); t[27] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLU_Max(dlu_hd); end = clock(); t[28] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLS_Max(dls_hd); end = clock(); t[29] = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock(); for (int k = 0; k < iter; k++) UA_Prd(D_ua, n, n - 1); end = clock(); t[30] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SA_Prd(D_sa, n, n - 1); end = clock(); t[31] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLU_Prd(slu_hd, slu_tl); end = clock(); t[32] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLS_Prd(sls_hd, sls_tl); end = clock(); t[33] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLU_Prd(dlu_hd, dlu_tl); end = clock(); t[34] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLS_Prd(dls_hd, dls_tl); end = clock(); t[35] = ((double)(end - start)) / CLOCKS_PER_SEC;

        start = clock(); for (int k = 0; k < iter; k++) UA_Suc(D_ua, n, 0); end = clock(); t[36] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SA_Suc(D_sa, n, 0); end = clock(); t[37] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLU_Suc(slu_hd, slu_hd); end = clock(); t[38] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) SLS_Suc(sls_hd, sls_hd); end = clock(); t[39] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLU_Suc(dlu_hd, dlu_hd); end = clock(); t[40] = ((double)(end - start)) / CLOCKS_PER_SEC;
        start = clock(); for (int k = 0; k < iter; k++) DLS_Suc(dls_hd, dls_hd); end = clock(); t[41] = ((double)(end - start)) / CLOCKS_PER_SEC;

        fprintf(fp, "%d", n);
        for (int x = 0; x < 42; x++) fprintf(fp, ",%f", t[x]);
        fprintf(fp, "\n");

        printf("Processed N = %d\n", n);
        free(D_ua);
        free(D_sa);
    }

    fclose(fp);
    printf("Results saved to benchmark.csv\n");
    return 0;
}
