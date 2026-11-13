#include <stdio.h>
#include <stdlib.h>

static void swp_int(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void heapify(int *arr, int n, int i) {
    int big = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[big]) {
        big = left;
    }

    if (right < n && arr[right] > arr[big]) {
        big = right;
    }

    if (big != i) {
        swp_int(&arr[i], &arr[big]);
        heapify(arr, n, big);
    }
}

static void h_sort(int *arr, int n) {
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(arr, n, i);
    }

    for (int i = n - 1; i > 0; --i) {
        swp_int(&arr[0], &arr[i]);
        heapify(arr, i, 0);
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

    if (!sav_arr("q04_input.txt", arr, n)) {
        printf("Could not write q04_input.txt\n");
        return 0;
    }

    printf("Original data: ");
    prt_arr(arr, n);

    h_sort(arr, n);

    printf("Sorted data:   ");
    prt_arr(arr, n);

    if (!sav_arr("q04_sorted.txt", arr, n)) {
        printf("Could not write q04_sorted.txt\n");
        return 0;
    }

    printf("Input saved to q04_input.txt\n");
    printf("Sorted output saved to q04_sorted.txt\n");

    return 0;
}
