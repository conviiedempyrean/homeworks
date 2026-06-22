#include <stdio.h>

#define ROWS 5
#define COLS 4

int row_sum(int (*row)[COLS]);
void sort_rows_by_sum(int (*arr)[COLS], int rows);

int main(void)
{
    int a[ROWS][COLS] = {
        {4, 2, 1, 7},
        {1, 0, 3, 2},
        {9, 5, 6, 1},
        {2, 2, 2, 2},
        {8, 0, 0, 1}
    };

    printf("排序前:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", a[i][j]);
        }
        printf("= %d\n", row_sum(&a[i]));
    }

    sort_rows_by_sum(a, ROWS);

    printf("\n排序后:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", a[i][j]);
        }
        printf("= %d\n", row_sum(&a[i]));
    }

    return 0;
}

int row_sum(int (*row)[COLS])
{
    int sum = 0;
    int *p = *row;

    for (int j = 0; j < COLS; j++) {
        sum += *(p + j);
    }

    return sum;
}

void sort_rows_by_sum(int (*arr)[COLS], int rows)
{
    for (int i = 0; i < rows - 1; i++) {
        for (int j = i + 1; j < rows; j++) {
            if (row_sum(&arr[i]) > row_sum(&arr[j])) {
                int temp[COLS];
                int (*row_i)[COLS] = &arr[i];
                int (*row_j)[COLS] = &arr[j];

                for (int k = 0; k < COLS; k++) {
                    temp[k] = (*row_i)[k];
                    (*row_i)[k] = (*row_j)[k];
                    (*row_j)[k] = temp[k];
                }
            }
        }
    }
}
    