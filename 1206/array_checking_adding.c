#include <stdio.h>

void array_input(double *arr,int size);
void array_output(double *arr,int size);
void array_sort(double *arr,int size);
void array_insertion(double *arr,int size,double ins);
int array_delete(double *arr,int size,double del);

int main()
{
    int based=0;
    double elem;
    int n;
    printf("input your array size:");
    scanf("%d",&n); 
    double array[n+1];
    array_input(array,n);
    array_sort(array,n);
    printf("input element you want check:");
    scanf("%lf",&elem);
    based=array_delete(array,n,elem);
    if(based==0)
    {
        array_insertion(array,n+1,elem);
    }
    array_output(array,n+1);
}

void array_input(double *arr,int size)
{
    printf("input your array elements,use spase to separate elements:\n");
    for(int i=0;i<size;i++)
    {
        scanf(" %lf",&arr[i]);
    }
}

void array_output(double *arr,int size)
{
    printf("your array:");
    for(int i=0;i<size;i++)
    {
        printf(" %.1f ",arr[i]);
    }
    printf("\n");
}

void array_sort(double *arr,int size)
{
    for (int i = 0; i < size; i++)
    {
        int min = i;
        for(int j = i+1;j<size;j++)
        {
            if(arr[j]<arr[min])
            {
                min=j;
            }
        }
        if(min!=i)
        {
            int change=arr[min];
            arr[min]=arr[i];
            arr[i]=change;
        }
    }
    printf("sort completed\n");
}

void array_insertion(double *arr,int size,double ins)
{
    int flag=0;
    int i=0;
    for(i=size-2;i>=0;i--)
    {
        if (arr[i]>ins)
        {
            arr[i+1]=arr[i];
        }
        else 
        {
            break;
        }
    }
    arr[i+1]=ins;
    printf("insert completed\n");
}

int array_delete(double *arr,int size,double del)
{
    int flag=0;
    int i=0;
    for(i=0;i<size;i++)
    {
        if(arr[i]==del)
        {
            for(int j=i;j<size;j++)
            {
                arr[j]=arr[j+1];
            }
            printf("delete completed\n");
            flag=1;
        }
        else
        {
            flag=0;
        }
    }
    printf("could not found\n");
    return flag;
}
