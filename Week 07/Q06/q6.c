#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int year;
    int type;
} Event;

int cmp_ev(const void *a, const void *b) {
    Event *ea = (Event *)a;
    Event *eb = (Event *)b;
    if (ea->year != eb->year) {
        return ea->year - eb->year;
    }
    return ea->type - eb->type;
}

void solve(int *birth, int *death, int n) {
    int n_ev = n * 2;
    Event *evts = malloc(n_ev * sizeof(Event));
    if (!evts) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        evts[2 * i].year = birth[i];
        evts[2 * i].type = 1;
        evts[2 * i + 1].year = death[i];
        evts[2 * i + 1].type = -1;
    }

    qsort(evts, n_ev, sizeof(Event), cmp_ev);

    int max_alv = 0;
    int cur_alv = 0;
    int bst_yr = evts[0].year;

    for (int i = 0; i < n_ev; i++) {
        cur_alv += evts[i].type;
        if (cur_alv > max_alv) {
            max_alv = cur_alv;
            bst_yr = evts[i].year;
        }
    }

    printf("\nRESULT: The best year to be alive was %d with %d scientists alive\n", bst_yr, max_alv);
    free(evts);
}

int main(void) {
    int birth[] = {1879, 1885, 1901, 1867, 1912, 1918, 1856};
    int death[] = {1955, 1962, 1976, 1934, 1954, 1988, 1943};
    int n = sizeof(birth) / sizeof(birth[0]);

    printf("The Best Time to Be Alive (n = %d scientists)\n", n);
    for (int i = 0; i < n; i++) {
        printf("Scientist %d: %d - %d\n", i + 1, birth[i], death[i]);
    }

    solve(birth, death, n);
    return 0;
}
