#include <stdio.h>
#include <stdlib.h>

#define DIM1 2
#define DIM2 3
#define DIM3 4

int sum_third_level(int ***p)
{
    int total = 0;
    for (int i = 0; i < DIM1; i++) {
        for (int j = 0; j < DIM2; j++) {
            for (int k = 0; k < DIM3; k++) {
                total += p[i][j][k];
            }
        }
    }
    return total;
}

int main(void)
{
    int ***p = malloc(DIM1 * sizeof(int **));
    if (p == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 0; i < DIM1; i++) {
        p[i] = malloc(DIM2 * sizeof(int *));
        if (p[i] == NULL) 
        {
            perror("malloc");
            for (int x = 0; x < i; x++) 
            {
                free(p[x]);
            }
            free(p);
            return 1;
        }
        for (int j = 0; j < DIM2; j++) {
            p[i][j] = malloc(DIM3 * sizeof(int));
            if (p[i][j] == NULL) {
                perror("malloc");
                for (int y = 0; y < j; y++) {
                    free(p[i][y]);
                }
                for (int x = 0; x < i; x++) {
                    for (int y = 0; y < DIM2; y++) {
                        free(p[x][y]);
                    }
                    free(p[x]);
                }
                free(p[i]);
                free(p);
                return 1;
            }
            for (int k = 0; k < DIM3; k++) {
                p[i][j][k] = i * DIM2 * DIM3 + j * DIM3 + k + 1;
            }
        }
    }

    int result = sum_third_level(p);
    printf("sum = %d\n", result);

    for (int i = 0; i < DIM1; i++) {
        for (int j = 0; j < DIM2; j++) {
            free(p[i][j]);
        }
        free(p[i]);
    }
    free(p);

    return 0;
}
