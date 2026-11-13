#include <stdlib.h>

extern void merge(int *arr1, int size1, int *arr2, int size2, int *res);

int *m1_mrg(int **arrs, int k, int n) {
    if (k == 0) return NULL;

    int *res = (int *)malloc(k * n * sizeof(int));

    for (int i = 0; i < n; i++) {
        res[i] = arrs[0][i];
    }

    int cur_sz = n;

    for (int i = 1; i < k; i++) {
        int *temp = (int *)malloc((cur_sz + n) * sizeof(int));

        merge(res, cur_sz, arrs[i], n, temp);

        for (int j = 0; j < cur_sz + n; j++) {
            res[j] = temp[j];
        }

        free(temp);
        cur_sz += n;
    }

    return res;
}
