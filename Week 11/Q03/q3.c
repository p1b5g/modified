#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_N   100000
#define MAX_L   10
#define W_BUF   16
#define HT_SIZE 262144
#define EMPTY   -1

typedef struct {
    int n;
    int len;
    char start_w[W_BUF];
    char tgt_w[W_BUF];
    char (*words)[W_BUF];
    int *h_slot;
} WordDct;

static uint32_t hsh_wrd(const char *str, int len) {
    uint32_t h = 2166136261u;
    for (int i = 0; i < len; i++) {
        h ^= (uint8_t)str[i];
        h *= 16777619u;
    }
    return h & (HT_SIZE - 1);
}

WordDct *crt_dct(int n, int len) {
    WordDct *dict = (WordDct *)malloc(sizeof(WordDct));
    if (!dict) {
        perror("Failed to allocate WordDct");
        exit(EXIT_FAILURE);
    }
    dict->n = n;
    dict->len = len;
    dict->words = malloc((n + 1) * sizeof(*dict->words));
    dict->h_slot = (int *)malloc(HT_SIZE * sizeof(int));
    if (!dict->words || !dict->h_slot) {
        perror("Failed to allocate dictionary storage");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < HT_SIZE; i++) {
        dict->h_slot[i] = EMPTY;
    }
    return dict;
}

bool ins_wrd(WordDct *dict, const char *word, int idx) {
    uint32_t slot = hsh_wrd(word, dict->len);
    while (dict->h_slot[slot] != EMPTY) {
        int ex_idx = dict->h_slot[slot];
        if (strcmp(dict->words[ex_idx], word) == 0) {
            return false;
        }
        slot = (slot + 1) & (HT_SIZE - 1);
    }
    strcpy(dict->words[idx], word);
    dict->h_slot[slot] = idx;
    return true;
}

int lkp_wrd(const WordDct *dict, const char *word) {
    uint32_t slot = hsh_wrd(word, dict->len);
    while (dict->h_slot[slot] != EMPTY) {
        int idx = dict->h_slot[slot];
        if (strcmp(dict->words[idx], word) == 0) {
            return idx;
        }
        slot = (slot + 1) & (HT_SIZE - 1);
    }
    return -1;
}

void fre_dct(WordDct *dict) {
    if (!dict) return;
    free(dict->words);
    free(dict->h_slot);
    free(dict);
}

void prt_dct(const WordDct *dict) {
    printf("\n--- Word Graph Instance Summary ---\n");
    printf("Dictionary Size (N): %d | Word Length (L): %d\n", dict->n, dict->len);
    printf("Start Word (A): \"%s\" | Target Word (B): \"%s\"\n",
           dict->start_w, dict->tgt_w);
    printf("Dictionary Words D = { ");
    for (int i = 0; i < dict->n; i++) {
        printf("\"%s\"%s", dict->words[i], (i + 1 < dict->n) ? ", " : " ");
    }
    printf("}\n");
}

static int exp_lvl(const WordDct *dict,
                   int *q, int *front, int *back,
                   int *d_self, const int *d_oth,
                   int *p_self) {
    int lvl_sz = *back - *front;
    int len = dict->len;

    for (int i = 0; i < lvl_sz; i++) {
        int u = q[(*front)++];
        char mut[W_BUF];
        strcpy(mut, dict->words[u]);

        for (int pos = 0; pos < len; pos++) {
            char orig_c = mut[pos];
            for (char c = 'a'; c <= 'z'; c++) {
                if (c == orig_c) continue;
                mut[pos] = c;

                int v = lkp_wrd(dict, mut);
                if (v != -1) {
                    if (d_oth[v] != -1) {
                        if (d_self[v] == -1) {
                            d_self[v] = d_self[u] + 1;
                            p_self[v] = u;
                        }
                        return v;
                    }
                    if (d_self[v] == -1) {
                        d_self[v] = d_self[u] + 1;
                        p_self[v] = u;
                        q[(*back)++] = v;
                    }
                }
            }
            mut[pos] = orig_c;
        }
    }
    return -1;
}

static void prt_seq(const WordDct *dict,
                    int mt_idx,
                    const int *p_fwd,
                    const int *p_bwd) {
    int tot_nds = dict->n + 1;
    int *f_chn = (int *)malloc(tot_nds * sizeof(int));
    int f_len = 0;

    int curr = mt_idx;
    while (curr != -1) {
        f_chn[f_len++] = curr;
        curr = p_fwd[curr];
    }

    printf("Shortest Transformation Sequence: ");
    for (int i = f_len - 1; i >= 0; i--) {
        printf("%s", dict->words[f_chn[i]]);
        if (i > 0 || p_bwd[mt_idx] != -1) {
            printf(" -> ");
        }
    }

    curr = p_bwd[mt_idx];
    while (curr != -1) {
        printf("%s", dict->words[curr]);
        curr = p_bwd[curr];
        if (curr != -1) printf(" -> ");
    }
    printf("\n");

    free(f_chn);
}

int slv_bfs(WordDct *dict) {
    int tgt_idx = lkp_wrd(dict, dict->tgt_w);
    if (tgt_idx == -1) {
        printf("\n--- Bidirectional BFS Result ---\n");
        printf("Target word B = \"%s\" is NOT in dictionary D.\n", dict->tgt_w);
        printf("Minimum number of mutations k: -1 (No valid path exists)\n");
        return -1;
    }

    if (strcmp(dict->start_w, dict->tgt_w) == 0) {
        printf("\n--- Bidirectional BFS Result ---\n");
        printf("Start word A and Target word B are identical (\"%s\").\n", dict->start_w);
        printf("Minimum number of mutations k: 0\n");
        return 0;
    }

    int st_idx = lkp_wrd(dict, dict->start_w);
    if (st_idx == -1) {
        st_idx = dict->n;
        ins_wrd(dict, dict->start_w, st_idx);
    }

    int tot_slt = dict->n + 1;
    int *d_fwd = (int *)malloc(tot_slt * sizeof(int));
    int *d_bwd = (int *)malloc(tot_slt * sizeof(int));
    int *p_fwd = (int *)malloc(tot_slt * sizeof(int));
    int *p_bwd = (int *)malloc(tot_slt * sizeof(int));
    int *q_fwd = (int *)malloc(tot_slt * sizeof(int));
    int *q_bwd = (int *)malloc(tot_slt * sizeof(int));

    if (!d_fwd || !d_bwd || !p_fwd || !p_bwd || !q_fwd || !q_bwd) {
        perror("Failed to allocate Bidirectional BFS buffers");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < tot_slt; i++) {
        d_fwd[i] = -1;
        d_bwd[i] = -1;
        p_fwd[i] = -1;
        p_bwd[i] = -1;
    }

    int frt_f = 0, bck_f = 0;
    int frt_b = 0, bck_b = 0;

    d_fwd[st_idx] = 0;
    q_fwd[bck_f++] = st_idx;

    d_bwd[tgt_idx] = 0;
    q_bwd[bck_b++] = tgt_idx;

    int min_mut = -1;
    int mt_node = -1;

    while (frt_f < bck_f && frt_b < bck_b) {
        int sz_f = bck_f - frt_f;
        int sz_b = bck_b - frt_b;

        if (sz_f <= sz_b) {
            mt_node = exp_lvl(dict, q_fwd, &frt_f, &bck_f,
                              d_fwd, d_bwd, p_fwd);
        } else {
            mt_node = exp_lvl(dict, q_bwd, &frt_b, &bck_b,
                              d_bwd, d_fwd, p_bwd);
        }

        if (mt_node != -1) {
            min_mut = d_fwd[mt_node] + d_bwd[mt_node];
            break;
        }
    }

    printf("\n--- Bidirectional BFS Result ---\n");
    if (min_mut != -1) {
        printf("Minimum number of mutations k: %d\n", min_mut);
        prt_seq(dict, mt_node, p_fwd, p_bwd);
    } else {
        printf("Minimum number of mutations k: -1 (No valid transformation sequence exists)\n");
    }

    free(d_fwd);
    free(d_bwd);
    free(p_fwd);
    free(p_bwd);
    free(q_fwd);
    free(q_bwd);

    return min_mut;
}

int main(void) {
    printf("===========================================================================\n");
    printf("  Q03: Bidirectional BFS for Shortest Transformation Sequence in Word Graph\n");
    printf("===========================================================================\n");

    const char *w_list[] = {"hot", "dot", "dog", "lot", "log", "cog"};
    int n = 6, l = 3;
    WordDct *dict = crt_dct(n, l);
    strcpy(dict->start_w, "hit");
    strcpy(dict->tgt_w, "cog");
    for (int i = 0; i < n; i++) {
        ins_wrd(dict, w_list[i], i);
    }

    prt_dct(dict);
    slv_bfs(dict);
    fre_dct(dict);

    return EXIT_SUCCESS;
}
