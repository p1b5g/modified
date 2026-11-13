#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int num;
    char col;
} Item;

void prt_itm(const Item items[], int n, const char *label) {
    printf("%s: ", label);
    for (int i = 0; i < n; i++) {
        printf("%d%c ", items[i].num, items[i].col);
    }
    printf("\n");
}

void srt_col(Item items[], int n) {
    Item *red = (Item *)malloc((size_t)n * sizeof(Item));
    Item *blue = (Item *)malloc((size_t)n * sizeof(Item));
    Item *yel = (Item *)malloc((size_t)n * sizeof(Item));

    int r_cnt = 0, b_cnt = 0, y_cnt = 0;

    if (!red || !blue || !yel) {
        free(red); free(blue); free(yel);
        return;
    }

    for (int i = 0; i < n; i++) {
        if (items[i].col == 'R') red[r_cnt++] = items[i];
        else if (items[i].col == 'B') blue[b_cnt++] = items[i];
        else if (items[i].col == 'Y') yel[y_cnt++] = items[i];
    }

    int idx = 0;
    for (int i = 0; i < r_cnt; i++) items[idx++] = red[i];
    for (int i = 0; i < b_cnt; i++) items[idx++] = blue[i];
    for (int i = 0; i < y_cnt; i++) items[idx++] = yel[i];

    free(red); free(blue); free(yel);
}

int main(void) {
    Item items[] = {
        {1, 'B'}, {2, 'Y'}, {3, 'R'}, {5, 'B'},
        {7, 'R'}, {8, 'Y'}, {10, 'R'}, {12, 'B'}
    };
    int n = sizeof(items) / sizeof(items[0]);

    prt_itm(items, n, "Original items");
    srt_col(items, n);
    prt_itm(items, n, "Sorted by color");

    return 0;
}
