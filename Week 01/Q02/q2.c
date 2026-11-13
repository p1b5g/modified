#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int t_fair(void) {
    return rand() % 2;
}

int t_bias(double bias) {
    double r = (double)rand() / RAND_MAX;
    return (r < bias) ? 1 : 0;
}

int main(void) {
    srand((unsigned int)time(NULL));

    int max_t = 5000;
    double bias_p = 0.75;

    int f_heads = 0;
    int b_heads = 0;

    double f_probs[max_t + 1];
    double b_probs[max_t + 1];

    for (int i = 1; i <= max_t; i++) {
        f_heads += t_fair();
        b_heads += t_bias(bias_p);

        f_probs[i] = (double)f_heads / i;
        b_probs[i] = (double)b_heads / i;
    }

    printf("--- Simulation Complete (%d tosses) ---\n", max_t);
    printf("Expected Fair Coin Probability:   0.500000\n");
    printf("Calculated Fair Coin Probability: %f\n\n", f_probs[max_t]);
    printf("Expected Biased Coin Probability: %f\n", bias_p);
    printf("Calculated Biased Coin Prob:      %f\n", b_probs[max_t]);

    FILE *gp = popen("gnuplot -persistent", "w");
    if (gp == NULL) {
        return 0;
    }

    fprintf(gp, "set terminal pngcairo enhanced font 'arial,10' size 800,600\n");
    fprintf(gp, "set output 'coin_simulation.png'\n");
    fprintf(gp, "set title 'Law of Large Numbers: Fair vs Biased Coin'\n");
    fprintf(gp, "set xlabel 'Number of Tosses'\n");
    fprintf(gp, "set ylabel 'Cumulative Probability of Heads'\n");
    fprintf(gp, "set yrange [0:1]\n");
    fprintf(gp, "set key right bottom\n");

    fprintf(gp, "set arrow from 0,0.5 to %d,0.5 nohead lc rgb 'black' dashtype 2\n", max_t);
    fprintf(gp, "set arrow from 0,0.75 to %d,0.75 nohead lc rgb 'black' dashtype 2\n", max_t);

    fprintf(gp, "plot '-' title 'Fair Coin (Exp: 0.5)' with lines lc rgb 'blue', \\\n");
    fprintf(gp, "     '-' title 'Biased Coin (Exp: 0.75)' with lines lc rgb 'red'\n");

    int step = 10;

    for (int i = step; i <= max_t; i += step) {
        fprintf(gp, "%d %f\n", i, f_probs[i]);
    }
    fprintf(gp, "e\n");

    for (int i = step; i <= max_t; i += step) {
        fprintf(gp, "%d %f\n", i, b_probs[i]);
    }
    fprintf(gp, "e\n");

    pclose(gp);
    return 0;
}
