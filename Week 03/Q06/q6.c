#include <stdio.h>

void sel_srt(int A[], int n) {
    int i, j, min_idx, temp;

    for (i = 0; i < n - 1; i++) {
        min_idx = i;

        for (j = i + 1; j < n; j++) {
            if (A[j] < A[min_idx]) {
                min_idx = j;
            }
        }

        if (min_idx != i) {
            temp = A[i];
            A[i] = A[min_idx];
            A[min_idx] = temp;
        }
    }
}

void prt_arr(int A[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", A[i]);
    }
    printf("\n");
}

int main(void) {
    int A[] = {64, 25, 12, 22, 11};
    int n = sizeof(A) / sizeof(A[0]);

    printf("Original array: \n");
    prt_arr(A, n);

    sel_srt(A, n);

    printf("Sorted array: \n");
    prt_arr(A, n);

    return 0;
}
