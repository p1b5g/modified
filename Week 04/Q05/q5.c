#include <stdio.h>
#include <stdlib.h>

struct Intvl {
    int start;
    int end;
};

int cmp_iv(const void *a, const void *b) {
    return (((struct Intvl *)a)->start - ((struct Intvl *)b)->start);
}

struct Intvl *mrg_iv(struct Intvl arr[], int n, int *new_sz) {
    if (n <= 0) {
        *new_sz = 0;
        return NULL;
    }

    struct Intvl *copy = (struct Intvl *)malloc(n * sizeof(struct Intvl));
    for (int i = 0; i < n; i++) copy[i] = arr[i];

    qsort(copy, n, sizeof(struct Intvl), cmp_iv);

    struct Intvl *res = (struct Intvl *)malloc(n * sizeof(struct Intvl));
    int size = 0;
    res[size++] = copy[0];

    for (int i = 1; i < n; i++) {
        if (copy[i].start <= res[size - 1].end) {
            if (copy[i].end > res[size - 1].end) {
                res[size - 1].end = copy[i].end;
            }
        } else {
            res[size++] = copy[i];
        }
    }

    free(copy);
    *new_sz = size;
    return res;
}

int main(void) {
    struct Intvl arr[] = {{1, 3}, {2, 6}, {8, 10}, {7, 18}};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Original intervals: ");
    for (int i = 0; i < n; i++) {
        printf("(%d, %d) ", arr[i].start, arr[i].end);
    }
    printf("\n");

    int new_sz;
    struct Intvl *merged = mrg_iv(arr, n, &new_sz);

    printf("Merged intervals: ");
    for (int i = 0; i < new_sz; i++) {
        printf("(%d, %d) ", merged[i].start, merged[i].end);
    }
    printf("\n");

    free(merged);
    return 0;
}
