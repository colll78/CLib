#include <stdio.h>
#include <stdlib.h>

void shellsort(int v[], int n);
int binsearch(int x, int v[], int n);
void insertion_sort(int s[], int n);
void quicksort(int v[], int left, int right);
void swap(int *xp, int *yp);

int main()
{
    int sort_me[] = {35,21,51,11,5,111,74,32,126,31, 55, 81, 95, 16, 15,21,26};
    int len_sortme = sizeof(sort_me)/sizeof(sort_me[0]);
    quicksort(sort_me, 0, len_sortme-1);
    for(int i = 0; i < len_sortme; i++){
        printf("%d, ", sort_me[i]);
    }
    return 0;
}

void shellsort(int v[], int n)
{
    int gap, i, j, temp;

    for (gap = n/2; gap > 0; gap /= 2)
    {
        for (i = gap; i < n; i++)
        {
            for (j=i-gap; j >= 0 && v[j] > v[j+gap]; j -= gap)
            {
                temp = v[j];
                v[j] = v[j + gap];
                v[j+gap] = temp;
            }
        }
    }
}

/* binsearch: find x in sorted array v, v[0] <= v[1] <= ... <= v[n-1] */
int binsearch(int x, int v[], int n)
{
    int low, high, mid;

    low = 0;
    high = n-1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if ( x < v[mid])
        {
            high = mid - 1;
        }
        else if (x > v[mid])
        {
            low = mid + 1;
        }
        else
        {
            /* found x */
            return mid;
        }
    }
    return -1; /* no match was found */
}


void insertion_sort(int s[], int n)
{
    int i, j; /* counters */
    for (i = 1; i < n; i++)
    {
        j = i;
        while ((j>0) && (s[j] < s[j-1]))
        {
            swap(&s[j], &s[j-1]);
        }
    }
}

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void mergesort(int arr[], int leftStart, int rightEnd)
{
    if (leftStart >= rightEnd)
    {
        return;
    }
    int middle = (leftStart + rightEnd) / 2;
    mergesort(arr, leftStart, middle);
    mergesort(arr, middle, rightEnd);
    merge(arr, leftStart, middle, rightEnd);
}

void merge(int arr[], int leftStart, int middle, int rightEnd){
    int i;
    queue buffer1, buffer2;

    init_queue(&buffer1);
    init_queue(&buffer2);

    for(i = low; i <= middle; i++){ enqueue(&buffer1, arr[i]); }
    for(i = middle + 1; i <= righEnd; i++){ enqueue(&buffer2, arr[i]); }
    // TODO: finish this
}


/* quicksort: apply quicksort to v */
void quicksort(int v[], int left, int right)
{
    int i, last;
    if (left >= right){
        return;
    }
    swap(&v[left], &v[(left+right)/2]);
    last = left;
    for(i = left+1; i <= right; i++)
    {
        if(v[i] < v[left])
        {
            swap(&v[++last], &v[i]);
        }
    }
    swap(&v[left], &v[last]);
    quicksort(v, left, last-1);
    quicksort(v, last+1, right);
}
