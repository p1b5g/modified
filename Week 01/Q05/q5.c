#include <stdio.h>
#include <stdlib.h>

int fnd_prt(int arr[], int n) {
    int low = 0, high = n - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (arr[mid] == 1 && (mid == 0 || arr[mid - 1] == 0)) {
            return mid;
        } else if (arr[mid] == 1) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

int main(void) {
    int sz_arr = 20;
    int arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1};
    int trans = 12;

    printf("The actual transition point is at index: %d\n", trans);

    int idx = fnd_prt(arr, sz_arr);

    if (idx != -1)
        printf("-> Algorithm found partition point at index: %d\n", idx);
    else
        printf("-> Algorithm found no partition point (no 1s in array).\n");

    return 0;
}
