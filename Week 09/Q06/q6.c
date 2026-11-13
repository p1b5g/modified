#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100000
#define ALPHA   256

typedef struct {
    int count;
    unsigned char ch;
} Entry;

static Entry heap[ALPHA + 5];
static int hp_size;

static int better(Entry a, Entry b) {
    if (a.count != b.count) return a.count > b.count;
    return a.ch < b.ch;
}

static void hp_push(Entry e) {
    int i = hp_size++;
    heap[i] = e;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (!better(heap[i], heap[p])) break;
        Entry t = heap[i]; heap[i] = heap[p]; heap[p] = t;
        i = p;
    }
}

static Entry hp_pop(void) {
    Entry top = heap[0];
    heap[0] = heap[--hp_size];
    int i = 0;
    for (;;) {
        int l = 2 * i + 1, r = l + 1, m = i;
        if (l < hp_size && better(heap[l], heap[m])) m = l;
        if (r < hp_size && better(heap[r], heap[m])) m = r;
        if (m == i) break;
        Entry t = heap[i]; heap[i] = heap[m]; heap[m] = t;
        i = m;
    }
    return top;
}

static int reorg(const char *s, int k, char *out) {
    int n = (int)strlen(s);
    int cnt[ALPHA] = {0};
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;

    hp_size = 0;
    for (int c = 0; c < ALPHA; c++)
        if (cnt[c] > 0) hp_push((Entry){cnt[c], (unsigned char)c});

    Entry *queue = malloc((size_t)(n + 1) * sizeof(Entry));
    int qh = 0, qt = 0;

    for (int i = 0; i < n; i++) {
        if (hp_size == 0) { out[0] = '\0'; free(queue); return 0; }
        Entry e = hp_pop();
        out[i] = (char)e.ch;
        e.count--;
        queue[qt++] = e;
        if (qt - qh == k) {
            Entry back = queue[qh++];
            if (back.count > 0) hp_push(back);
        }
    }
    out[n] = '\0';
    free(queue);
    return 1;
}

static int arr_ok(const char *s, const char *t, int k) {
    int n = (int)strlen(s);
    if ((int)strlen(t) != n) return 0;
    int a[ALPHA] = {0}, b[ALPHA] = {0};
    for (int i = 0; i < n; i++) { a[(unsigned char)s[i]]++; b[(unsigned char)t[i]]++; }
    if (memcmp(a, b, sizeof a) != 0) return 0;
    int last[ALPHA];
    for (int c = 0; c < ALPHA; c++) last[c] = -1000000000;
    for (int i = 0; i < n; i++) {
        unsigned char c = (unsigned char)t[i];
        if (i - last[c] < k) return 0;
        last[c] = i;
    }
    return 1;
}

static int brute(int *cnt, int *last, int pos, int n, int k) {
    if (pos == n) return 1;
    for (int c = 0; c < ALPHA; c++) {
        if (cnt[c] == 0 || pos - last[c] < k) continue;
        int old = last[c];
        cnt[c]--; last[c] = pos;
        int ok = brute(cnt, last, pos + 1, n, k);
        cnt[c]++; last[c] = old;
        if (ok) return 1;
    }
    return 0;
}

static int brt_ok(const char *s, int k) {
    int cnt[ALPHA] = {0}, last[ALPHA];
    int n = (int)strlen(s);
    for (int i = 0; i < n; i++) cnt[(unsigned char)s[i]]++;
    for (int c = 0; c < ALPHA; c++) last[c] = -1000000000;
    return brute(cnt, last, 0, n, k);
}

int main(void) {
    static char s[MAX_LEN + 10] = "aabbccdd";
    static char out[MAX_LEN + 10];
    int k = 3;

    printf("=== Q06: Reorganise String with K-Distance Apart ===\n");
    printf("\nInput: S = \"%s\", K = %d\n", s, k);

    int ok = reorg(s, k, out);
    if (ok) printf("Result: \"%s\"\n", out);
    else printf("Result: \"\"  (impossible)\n");

    printf("\nValidation\n");
    if (ok) printf("  valid permutation with equal characters >= K apart -> %s\n",
                   arr_ok(s, out, k) ? "PASSED" : "FAILED");
    int feas = brt_ok(s, k);
    printf("  exhaustive search says feasible = %s -> %s\n", feas ? "yes" : "no",
           feas == ok ? "PASSED (agrees with greedy)" : "FAILED");
    return 0;
}
