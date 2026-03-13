#include <stdio.h>
#include <stdlib.h>

void input_scores(double **scores, int m, int n);
void calc_averages(double **scores, double *avg, int m, int n);
void array_sort(double *arr, int size);
void array_output(double *arr, int size);

int main() 
{
    int m, n;
    printf("Enter number of students m: ");
    scanf("%d", &m);
    printf("Enter number of courses n: ");
    scanf("%d", &n);
    
    double **scores = (double **)malloc(m * sizeof(double *));
    for (int i = 0; i < m; i++) 
    {
        scores[i] = (double *)malloc(n * sizeof(double));
    }
    
    double *averages = (double *)malloc(m * sizeof(double));
    
    input_scores(scores, m, n);
    calc_averages(scores, averages, m, n);
    
    printf("\n===== Original Averages =====\n");
    array_output(averages, m);
    
    array_sort(averages, m);
    
    printf("\n===== Sorted Averages (Descending) =====\n");
    array_output(averages, m);
    
    free_matrix(scores,m);
    free(averages);
    
    return 0;
}

double **create_matrix(int m, int n)
{
    double **scores = (double **)malloc(m * sizeof(double *));
    if (scores == NULL) return NULL;
    
    for (int i = 0; i < m; i++)
    {
        scores[i] = (double *)malloc(n * sizeof(double));
        if (scores[i] == NULL) {
            for (int j = 0; j < i; j++)
            {
                free(scores[j]);
            }
            free(scores);
            return NULL;
        }
    }
    return scores;
}

void free_matrix(double **scores, int m) {
    for (int i = 0; i < m; i++) {
        free(scores[i]);
    }
    free(scores);
}

void input_scores(double **scores, int m, int n)
{
    printf("\nEnter scores for %d students and %d courses:\n", m, n);
    for (int i = 0; i < m; i++) 
    {
        printf("Student %d: ", i + 1);
        for (int j = 0; j < n; j++) 
        {
            scanf("%lf", &scores[i][j]);
        }
    }
}

void calc_averages(double **scores, double *avg, int m, int n)
{
    for (int i = 0; i < m; i++) 
    {
        double sum = 0.0;
        for (int j = 0; j < n; j++) 
        {
            sum += scores[i][j];
        }
        avg[i] = sum / n;
    }
    printf("averages calculated\n");
}

void array_sort(double *arr, int size) 
{
    for (int i = 0; i < size - 1; i++) 
    {
        int max = i;
        for (int j = i + 1; j < size; j++) 
        {
            if (arr[j] > arr[max]) 
            {
                max = j;
            }
        }
        if (max != i) 
        {
            double temp = arr[max];
            arr[max] = arr[i];
            arr[i] = temp;
        }
    }
    printf("sort completed\n");
}

void array_output(double *arr, int size) 
{
    printf("your array:");
    for (int i = 0; i < size; i++) 
    {
        printf(" %.2f ", arr[i]);
    }
    printf("\n");
}