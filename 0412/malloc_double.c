#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int g_m = 0;

static double **allocate_data(int n, int m)
{
    double **data = malloc(n * sizeof(double *));
    if (data == NULL) {
        fprintf(stderr, "Failed to allocate pointer array for %d blocks.\n", n);
        return NULL;
    }
    for (int i = 0; i < n; ++i) {
        data[i] = malloc(m * sizeof(double));
        if (data[i] == NULL) {
            fprintf(stderr, "Failed to allocate block %d of %d doubles.\n", i, m);
            for (int j = 0; j < i; ++j) {
                free(data[j]);
            }
            free(data);
            return NULL;
        }

        for (int j = 0; j < m; ++j) {
            data[i][j] = (rand() % 10000) / 100.0;
        }
    }

    return data;
}

static void free_data(double **data, int n)
{
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        free(data[i]);
    }
    free(data);
}

static double sum_block(const double *block, int m)
{
    double sum = 0.0;
    for (int i = 0; i < m; ++i) {
        sum += block[i];
    }
    return sum;
}

static int compare_by_sum(const void *a, const void *b)
{
    const double *const *pa = a;
    const double *const *pb = b;

    double sa = sum_block(*pa, g_m);
    double sb = sum_block(*pb, g_m);

    if (sa < sb) {
        return -1;
    }
    if (sa > sb) {
        return 1;
    }
    return 0;
}

static void sort_data(double **data, int n, int m)
{
    if (data == NULL || n <= 1 || m <= 0) {
        return;
    }

    g_m = m;
    qsort(data, n, sizeof(double *), compare_by_sum);
}

static void print_data(double **data, int n, int m)
{
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            printf("%.2f", data[i][j]);
            if (j < m - 1) {
                putchar(' ');
            }
        }
        putchar('\n');
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s n m\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    if (n <= 0 || m <= 0) {
        fprintf(stderr, "Invalid n or m. Both must be positive integers.\n");
        return EXIT_FAILURE;
    }

    srand((unsigned)time(NULL));

    double **data = allocate_data(n, m);
    if (data == NULL) {
        return EXIT_FAILURE;
    }

    sort_data(data, n, m);
    print_data(data, n, m);
    free_data(data, n);

    return EXIT_SUCCESS;
}
