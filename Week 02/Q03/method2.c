#include <stdlib.h>

extern void merge(int *arr1, int size1, int *arr2, int size2, int *res);

int *m2_rec(int **arrs, int l, int r, int n, int *out_sz) {
    if (l == r) {
        *out_sz = n;
        int *res = (int *)malloc(n * sizeof(int));
        for (int i = 0; i < n; i++) res[i] = arrs[l][i];
        return res;
    }

    int mid = l + (r - l) / 2;
    int sz_l, sz_r;

    int *left = m2_rec(arrs, l, mid, n, &sz_l);
    int *right = m2_rec(arrs, mid + 1, r, n, &sz_r);

    *out_sz = sz_l + sz_r;
    int *merged = (int *)malloc((*out_sz) * sizeof(int));

    merge(left, sz_l, right, sz_r, merged);

    free(left);
    free(right);

    return merged;
}

int *m2_mrg(int **arrs, int k, int n) {
    if (k == 0) return NULL;
    int fin_sz;
    return m2_rec(arrs, 0, k - 1, n, &fin_sz);
}
