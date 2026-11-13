#include <stdio.h>
#include <stdlib.h>

int cmp_int(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int max_ppl(int entry[], int exit_t[], int n, int *t_max) {
    int *ent_cpy = (int *)malloc(n * sizeof(int));
    int *ext_cpy = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        ent_cpy[i] = entry[i];
        ext_cpy[i] = exit_t[i];
    }

    qsort(ent_cpy, n, sizeof(int), cmp_int);
    qsort(ext_cpy, n, sizeof(int), cmp_int);

    int max_cnt = 0, cur_cnt = 0;
    int i = 0, j = 0;
    *t_max = -1;

    while (i < n && j < n) {
        if (ent_cpy[i] <= ext_cpy[j]) {
            cur_cnt++;
            if (cur_cnt > max_cnt) {
                max_cnt = cur_cnt;
                *t_max = ent_cpy[i];
            }
            i++;
        } else {
            cur_cnt--;
            j++;
        }
    }

    free(ent_cpy);
    free(ext_cpy);
    return max_cnt;
}

int main(void) {
    int entry[] = {1, 2, 5, 5, 8, 9};
    int exit_t[] = {4, 10, 9, 12, 11, 15};
    int n = sizeof(entry) / sizeof(entry[0]);
    int t_max;

    printf("Entry times: ");
    for (int i = 0; i < n; i++)
        printf("%d ", entry[i]);

    printf("\nExit times: ");
    for (int i = 0; i < n; i++)
        printf("%d ", exit_t[i]);

    printf("\n\n");

    int max_cnt = max_ppl(entry, exit_t, n, &t_max);
    printf("The most people present simultaneously is %d, which occurred at time %d.\n", max_cnt, t_max);

    return 0;
}
