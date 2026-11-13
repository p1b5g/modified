#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_SYM 256
#define MAX_NDS (2 * MAX_SYM)

typedef struct {
    long long freq;
    int left, right;
    int sym_idx;
} Node;

typedef struct {
    int n;
    char sym[MAX_SYM];
    long long freq[MAX_SYM];
} Input;

typedef struct {
    int len[MAX_SYM];
    char code[MAX_SYM][MAX_SYM + 2];
    int order[MAX_SYM];
} CBook;

static Node tree[MAX_NDS];
static int nd_cnt;
static int heap[MAX_NDS];
static int hp_size;

static int hp_less(int a, int b) {
    if (tree[a].freq != tree[b].freq) return tree[a].freq < tree[b].freq;
    return a < b;
}

static void hp_push(int v) {
    int i = hp_size++;
    heap[i] = v;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!hp_less(heap[i], heap[p])) break;
        int t = heap[i]; heap[i] = heap[p]; heap[p] = t;
        i = p;
    }
}

static int hp_pop(void) {
    int top = heap[0];
    heap[0] = heap[--hp_size];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hp_size && hp_less(heap[l], heap[m])) m = l;
        if (r < hp_size && hp_less(heap[r], heap[m])) m = r;
        if (m == i) break;
        int t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static int bld_huf(const Input *in) {
    nd_cnt = 0;
    hp_size = 0;
    for (int i = 0; i < in->n; i++) {
        tree[nd_cnt] = (Node){in->freq[i], -1, -1, i};
        hp_push(nd_cnt++);
    }
    while (hp_size > 1) {
        int a = hp_pop();
        int b = hp_pop();
        tree[nd_cnt] = (Node){tree[a].freq + tree[b].freq, a, b, -1};
        hp_push(nd_cnt++);
    }
    return hp_pop();
}

static void asn_len(int root, int depth, CBook *cb) {
    if (tree[root].left < 0) {
        cb->len[tree[root].sym_idx] = depth;
        return;
    }
    asn_len(tree[root].left, depth + 1, cb);
    asn_len(tree[root].right, depth + 1, cb);
}

static const Input *g_in;
static const CBook *g_cb;

static int cmp_can(const void *a, const void *b) {
    int i = *(const int *)a, j = *(const int *)b;
    if (g_cb->len[i] != g_cb->len[j]) return g_cb->len[i] - g_cb->len[j];
    return (unsigned char)g_in->sym[i] - (unsigned char)g_in->sym[j];
}

static void inc_bin(char *s, int len) {
    for (int i = len - 1; i >= 0; i--) {
        if (s[i] == '0') { s[i] = '1'; return; }
        s[i] = '0';
    }
}

static void asn_can(const Input *in, CBook *cb) {
    for (int i = 0; i < in->n; i++) cb->order[i] = i;
    g_in = in;
    g_cb = cb;
    qsort(cb->order, (size_t)in->n, sizeof(int), cmp_can);

    char cur[MAX_SYM + 2];
    int cur_len = 0;
    for (int k = 0; k < in->n; k++) {
        int s = cb->order[k];
        int L = cb->len[s];
        if (k == 0) {
            memset(cur, '0', (size_t)L);
        } else {
            inc_bin(cur, cur_len);
            for (int p = cur_len; p < L; p++) cur[p] = '0';
        }
        cur[L] = '\0';
        cur_len = L;
        strcpy(cb->code[s], cur);
    }
}

static void bld_cb(const Input *in, CBook *cb) {
    memset(cb, 0, sizeof(*cb));
    if (in->n == 1) {
        cb->len[0] = 1;
    } else {
        int root = bld_huf(in);
        asn_len(root, 0, cb);
    }
    asn_can(in, cb);
}

static void prt_cb(const Input *in, const CBook *cb) {
    long long total = 0, bits = 0;
    printf("\nCanonical Huffman codebook\n");
    printf("%8s %10s %7s  %s\n", "symbol", "frequency", "length", "code");
    for (int k = 0; k < in->n; k++) {
        int s = cb->order[k];
        printf("%8c %10lld %7d  %s\n", in->sym[s], in->freq[s], cb->len[s], cb->code[s]);
        total += in->freq[s];
        bits += in->freq[s] * cb->len[s];
    }
    printf("\nTotal weighted length (bits) : %lld\n", bits);
    printf("Expected code length         : %.6f bits/symbol\n", (double)bits / (double)total);
}

static int val_cb(const Input *in, const CBook *cb) {
    int ok = 1;
    for (int i = 0; i < in->n && ok; i++)
        for (int j = 0; j < in->n; j++) {
            if (i == j) continue;
            if (strncmp(cb->code[i], cb->code[j], (size_t)cb->len[i]) == 0 && cb->len[i] <= cb->len[j]) {
                ok = 0;
                break;
            }
        }
    int prf_ok = ok;
    long double kraft = 0;
    for (int i = 0; i < in->n; i++) kraft += powl(2.0L, -(long double)cb->len[i]);
    int krf_ok = (in->n == 1) || fabsl(kraft - 1.0L) < 1e-9L;
    long long bits = 0, intrn = 0;
    for (int i = 0; i < in->n; i++) bits += in->freq[i] * cb->len[i];
    for (int i = 0; i < nd_cnt; i++) if (tree[i].left >= 0) intrn += tree[i].freq;
    int cst_ok = (in->n == 1) || bits == intrn;
    int can_ok = 1;
    for (int k = 1; k < in->n; k++) {
        int a = cb->order[k - 1], b = cb->order[k];
        if (cb->len[a] > cb->len[b]) can_ok = 0;
        char pa[MAX_SYM + 2], pb[MAX_SYM + 2];
        int L = cb->len[b];
        strcpy(pa, cb->code[a]);
        for (int p = cb->len[a]; p < L; p++) pa[p] = '0';
        pa[L] = '\0';
        strcpy(pb, cb->code[b]);
        if (strcmp(pa, pb) >= 0) can_ok = 0;
    }
    printf("\nValidation\n");
    printf("  prefix-free property         : %s\n", prf_ok ? "PASSED" : "FAILED");
    printf("  Kraft sum equals 1           : %s\n", krf_ok ? "PASSED" : "FAILED");
    printf("  cost equals sum of internal  : %s\n", cst_ok ? "PASSED" : "FAILED");
    printf("  canonical (sorted) ordering  : %s\n", can_ok ? "PASSED" : "FAILED");
    return prf_ok && krf_ok && cst_ok && can_ok;
}

int main(void) {
    static Input in = {
        6,
        {'a', 'b', 'c', 'd', 'e', 'f'},
        {45, 13, 12, 16, 9, 5}
    };
    static CBook cb;

    printf("=== Q02: Huffman Coding (canonical codebook) ===\n");
    printf("\nInput (n = %d)\n", in.n);
    for (int i = 0; i < in.n; i++) printf("  '%c' : %lld\n", in.sym[i], in.freq[i]);

    bld_cb(&in, &cb);
    prt_cb(&in, &cb);
    val_cb(&in, &cb);
    return 0;
}
