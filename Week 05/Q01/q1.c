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

static double med_uns(int *arr, int n) {
    int *cpy_one = malloc((size_t)n * sizeof(int));

    if (cpy_one == NULL) {
        return 0.0;
    }

    for (int i = 0; i < n; ++i) {
        cpy_one[i] = arr[i];
    }

    if (n % 2 == 1) {
        int mid = n / 2;
        int med = qselect(cpy_one, 0, n - 1, mid);
        free(cpy_one);
        return (double)med;
    }

    int *cpy_two = malloc((size_t)n * sizeof(int));

    if (cpy_two == NULL) {
        free(cpy_one);
        return 0.0;
    }

    for (int i = 0; i < n; ++i) {
        cpy_two[i] = arr[i];
    }

    int l_mid = qselect(cpy_one, 0, n - 1, (n / 2) - 1);
    int r_mid = qselect(cpy_two, 0, n - 1, n / 2);

    free(cpy_one);
    free(cpy_two);

    return (l_mid + r_mid) / 2.0;
}

static void prt_arr(const int *arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int arr[] = {12, 3, 5, 7, 4, 19, 26, 15};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Dataset: ");
    prt_arr(arr, n);

    printf("Median: %.2f\n", med_uns(arr, n));

    return 0;
}
