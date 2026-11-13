#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern int *m1_mrg(int **arrs, int k, int n);
extern int *m2_mrg(int **arrs, int k, int n);

void merge(int *arr1, int size1, int *arr2, int size2, int *res) {
    int i = 0, j = 0, k = 0;

    while (i < size1 && j < size2) {
        if (arr1[i] <= arr2[j]) {
            res[k++] = arr1[i++];
        } else {
            res[k++] = arr2[j++];
        }
    }

    while (i < size1) {
        res[k++] = arr1[i++];
    }
    while (j < size2) {
        res[k++] = arr2[j++];
    }
}

int **gen_arr(int k, int n) {
    int **arrs = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; i++) {
        arrs[i] = (int *)malloc(n * sizeof(int));
        arrs[i][0] = rand() % 100;

        for (int j = 1; j < n; j++) {
            arrs[i][j] = arrs[i][j - 1] + (rand() % 15) + 1;
        }
    }
    return arrs;
}

void anl_tm(int **arrs, int k, int n) {
    clock_t start, end;
    double t_m1, t_m2;

    printf("Total Arrays (K) : %d\n", k);
    printf("Elements per Array (N) : %d\n", n);
    printf("Total Elements to merge: %d\n\n", k * n);

    start = clock();
    int *res1 = m1_mrg(arrs, k, n);
    end = clock();
    t_m1 = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Method 1 (Sequential) Time : %f seconds\n", t_m1);

    start = clock();
    int *res2 = m2_mrg(arrs, k, n);
    end = clock();
    t_m2 = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Method 2 (Divide & Conquer) Time: %f seconds\n", t_m2);

    if (t_m2 < t_m1) {
        printf("\nResult: Divide & Conquer was faster by %f seconds.\n", t_m1 - t_m2);
    } else {
        printf("\nResult: Sequential was faster by %f seconds.\n", t_m2 - t_m1);
    }

    free(res1);
    free(res2);
}

int main(void) {
    int k = 100;
    int n = 500;

    srand(time(NULL));

    int **arrs = gen_arr(k, n);
    anl_tm(arrs, k, n);

    for (int i = 0; i < k; i++) {
        free(arrs[i]);
    }

    free(arrs);
    return 0;
}
