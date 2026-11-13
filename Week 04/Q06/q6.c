#include <stdio.h>
#include <stdlib.h>

struct Intvl {
    int start;
    int end;
};

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int fnd_pt(struct Intvl S[], int n) {
    int *starts = (int *)malloc(n * sizeof(int));
    int *ends = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        starts[i] = S[i].start;
        ends[i] = S[i].end;
    }

    qsort(starts, n, sizeof(int), cmp_int);
    qsort(ends, n, sizeof(int), cmp_int);

    int i = 0, j = 0;
    int cur_cnt = 0, max_cnt = 0;
    int res = starts[0];

    while (i < n && j < n) {
        if (starts[i] <= ends[j]) {
            cur_cnt++;
            if (cur_cnt > max_cnt) {
                max_cnt = cur_cnt;
                res = starts[i];
            }
            i++;
        } else {
            cur_cnt--;
            j++;
        }
    }

    free(starts);
    free(ends);
    return res;
}

int main(void) {
    struct Intvl S[] = {{10, 40}, {20, 60}, {50, 90}, {15, 70}};
    int n = sizeof(S) / sizeof(S[0]);

    printf("Original intervals: ");
    for (int i = 0; i < n; i++) {
        printf("(%d, %d) ", S[i].start, S[i].end);
    }
    printf("\n");

    int point = fnd_pt(S, n);
    printf("A point in the largest number of intervals is %d\n", point);

    return 0;
}
