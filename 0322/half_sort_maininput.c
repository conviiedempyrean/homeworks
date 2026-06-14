#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void selectionSort(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}

int binarySearch(int arr[], int n, int key)
{
    int low = 0;
    int high = n - 1;

    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key)
        {
            return mid;
        } else if (arr[mid] < key)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("用法: %s n key\n", argv[0]);
        printf("例如: %s 10 42\n", argv[0]);
        return 1;
    }

    char *endptr;
    int n = (int)strtol(argv[1], &endptr, 10);
    if (*endptr != '\0' || n <= 0)
    {
        printf("错误: n 必须是正整数\n");
        return 1;
    }

    int key = (int)strtol(argv[2], &endptr, 10);
    if (*endptr != '\0')
    {
        printf("错误: key 必须是整数\n");
        return 1;
    }

    int *arr = malloc(n * sizeof(int));
    if (arr == NULL)
    {
        printf("内存分配失败\n");
        return 1;
    }

    srand((unsigned)time(NULL));
    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100;
    }

    printf("原始数组: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    selectionSort(arr, n);

    printf("排序后数组: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    int index = binarySearch(arr, n, key);
    if (index >= 0)
    {
        printf("元素 %d 在排序后数组中的下标为 %d\n", key, index);
    } else
    {
        printf("元素 %d 未找到\n", key);
    }

    free(arr);
    return 0;
}
