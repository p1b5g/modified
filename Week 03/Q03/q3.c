#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int min;
    int max;
} Pair;

Pair get_mm(int arr[], int low, int high, int *comps) {
    Pair res, left, right;
    int mid;

    if (low == high) {
        res.max = arr[low];
        res.min = arr[low];
        return res;
    }

    if (high == low + 1) {
        (*comps)++;
        if (arr[low] > arr[high]) {
            res.max = arr[low];
            res.min = arr[high];
        } else {
            res.max = arr[high];
            res.min = arr[low];
        }
        return res;
    }

    mid = (low + high) / 2;
    left = get_mm(arr, low, mid, comps);
    right = get_mm(arr, mid + 1, high, comps);

    (*comps) += 2;

    if (left.max > right.max) {
        res.max = left.max;
    } else {
        res.max = right.max;
    }

    if (left.min < right.min) {
        res.min = left.min;
    } else {
        res.min = right.min;
    }

    return res;
}

int main(void) {
    int arr[] = {42, 15, 78, 3, 99, 24, 61, 8};
    int n = sizeof(arr) / sizeof(arr[0]);
    int cmps = 0;

    printf("Array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n\n");

    Pair minmax = get_mm(arr, 0, n - 1, &cmps);

    printf("Minimum element is %d\n", minmax.min);
    printf("Maximum element is %d\n", minmax.max);
    printf("\n");
    printf("Elements in Array (n): %d\n", n);
    printf("Theoretical formula (3n/2 - 2): %d\n", (3 * n / 2) - 2);
    printf("Actual comparisons made: %d\n", cmps);

    return 0;
}
