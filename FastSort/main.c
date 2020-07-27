#include <stdio.h>
#include <malloc.h>

int sort (int *array, int first, int last){   //recursive sorting
    int X;
    if (first < last){
        X = swap(array, first, last);
        sort(array, first, X - 1);
        sort(array, X, last);
    }
    else
        return (int) array;
}

int swap (int *array, int first, int last){    //exchange of elements by algorithm
    int C;
int base = array[first];
int  i = first - 1;
    int j = last + 1;
    while (i < j - 1){
        do
            i ++;
        while (*(array + i) < base);
        do
            j --;
        while (*(array + j) > base);

        if (i < j) {
            C = array[i];
            array[i] = array[j];
            array[j] = C;
        }
        else
            j++;
    }
    return j;
    }

int main(){
    int N, i;
    int *array, last, first;

    FILE *fin, *fout;
    fin = fopen("in.txt", "r");
    if (! fin) {
        puts("error io");
        return 0;
    }
    fout = fopen("out.txt", "w");
    fscanf(fin, "%d", &N);
    array = (int *) malloc (N *sizeof(int));

    if (array == NULL){
        fprintf(fout, "Error:Out of memory");
        return 1;
    }
    for (i = 0; i < N; i++)
        fscanf(fin, "%d", array + i);

        last = N - 1;
        first = 0;
        array = (int *) sort (array, first, last);
    for (i = 0; i < N; i++)
        fprintf(fout, "%d ", *(array + i));

    free (malloc);
    return 0;
}