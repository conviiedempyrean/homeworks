#include <stdio.h>
#include <stdlib.h>

#define X 3
#define Y 3
#define Z 4

static inline int idx(int i, int j, int k)
{
    return i * Y * Z + j * Z + k;
}

int sum_layer(int *arr, int x, int y, int z, int layer);
int max_sum_layer(int *arr, int x, int y, int z);

int main(void)
{
    int *a = malloc(X * Y * Z * sizeof *a);
    if (a == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return EXIT_FAILURE;
    }

    int values[X][Y][Z] = {
        {
            {1, 2, 3, 4},
            {5, 6, 7, 8},
            {9, 0, 1, 2}
        },
        {
            {2, 2, 2, 2},
            {2, 2, 2, 2},
            {2, 2, 2, 2}
        },
        {
            {9, 8, 7, 6},
            {5, 4, 3, 2},
            {1, 0, 1, 0}
        }
    };

    for (int i = 0; i < X; i++) {
        for (int j = 0; j < Y; j++) {
            for (int k = 0; k < Z; k++) {
                a[idx(i, j, k)] = values[i][j][k];
            }
        }
    }

    int best_layer = max_sum_layer(a, X, Y, Z);
    printf("最大层和所在层下标: %d\n", best_layer);
    printf("该层元素和: %d\n", sum_layer(a, X, Y, Z, best_layer));

    free(a);
    return 0;
}

int sum_layer(int *arr, int x, int y, int z, int layer)
{
    int sum = 0;
    int base = layer * y * z;

    for (int j = 0; j < y; j++) {
        for (int k = 0; k < z; k++) {
            sum += arr[base + j * z + k];
        }
    }

    return sum;
}

int max_sum_layer(int *arr, int x, int y, int z)
{
    int max_index = 0;
    int max_sum = sum_layer(arr, x, y, z, 0);

    for (int i = 1; i < x; i++) {
        int current_sum = sum_layer(arr, x, y, z, i);
        if (current_sum > max_sum) {
            max_sum = current_sum;
            max_index = i;
        }
    }

    return max_index;
}
