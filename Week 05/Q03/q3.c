#include <stdio.h>
#include <stdlib.h>

static void swp_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int part(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            swp_int(&arr[i], &arr[j]);
        }
    }

    swp_int(&arr[i + 1], &arr[high]);
    return i + 1;
}

static void q_sort(int *arr, int low, int high) {
    if (low < high) {
        int piv_idx = part(arr, low, high);
        q_sort(arr, low, piv_idx - 1);
        q_sort(arr, piv_idx + 1, high);
    }
}

static void prt_arr(const int *arr, int n) {
    for (int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

static int sav_arr(const char *fname, const int *arr, int n) {
    FILE *file = fopen(fname, "w");

    if (file == NULL) {
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        fprintf(file, "%d ", arr[i]);
    }

    fprintf(file, "\n");
    fclose(file);
    return 1;
}

int main(void) {
    int arr[] = {42, 17, 93, 8, 55, 29, 71, 64, 11, 80};
    int n = sizeof(arr) / sizeof(arr[0]);

    if (!sav_arr("q03_input.txt", arr, n)) {
        printf("Could not write q03_input.txt\n");
        return 0;
    }

    printf("Original data: ");
    prt_arr(arr, n);

    q_sort(arr, 0, n - 1);

    printf("Sorted data:   ");
    prt_arr(arr, n);

    if (!sav_arr("q03_sorted.txt", arr, n)) {
        printf("Could not write q03_sorted.txt\n");
        return 0;
    }

    printf("Input saved to q03_input.txt\n");
    printf("Sorted output saved to q03_sorted.txt\n");

    return 0;
}
