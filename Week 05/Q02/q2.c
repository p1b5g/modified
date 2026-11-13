#include <stdio.h>
#include <stdlib.h>

static void swp_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int part(int *arr, int left, int right) {
    int pivot = arr[right];
    int st_idx = left;

    for (int i = left; i < right; ++i) {
        if (arr[i] <= pivot) {
            swp_int(&arr[st_idx], &arr[i]);
            ++st_idx;
        }
    }

    swp_int(&arr[st_idx], &arr[right]);
    return st_idx;
}

static int qselect(int *arr, int left, int right, int k_idx) {
    while (left <= right) {
        int piv_idx = part(arr, left, right);

        if (piv_idx == k_idx) {
            return arr[piv_idx];
        }

        if (piv_idx < k_idx) {
            left = piv_idx + 1;
        } else {
            right = piv_idx - 1;
        }
    }

    return -1;
}

static void prt_arr(const int *arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int arr[] = {12, 3, 5, 7, 4, 19, 26};
    int n = sizeof(arr) / sizeof(arr[0]);
    int k = 3;

    printf("Dataset: ");
    prt_arr(arr, n);

    int *copy = malloc((size_t)n * sizeof(int));
    if (copy == NULL) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        copy[i] = arr[i];
    }

    printf("%d-th smallest element: %d\n", k, qselect(copy, 0, n - 1, k - 1));

    free(copy);
    return 0;
}
