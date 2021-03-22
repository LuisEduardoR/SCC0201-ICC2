# include <stdio.h>
#include <stdlib.h>
# include <time.h>

int sequentialSearch (int* vet, int size, int target);
int binarySearch (int* vet, int size, int target);
int* createVector (unsigned size);
int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char const *argv[])
{

    unsigned vet_size;
    int* vector = NULL;

    printf("Vector size:\n");
    scanf(" %d", &vet_size);

    srand(time(NULL));

    vector = createVector(vet_size);

    int target;
    printf("Target:\n");
    scanf(" %d", &target);

    int pos;
    clock_t clock_start, clock_end;

    printf("Sequential:\n");

    clock_start = clock();
    pos = sequentialSearch(vector, vet_size, target);
    clock_end = clock();

    printf("> Done after %lf seconds.\n", (double)(clock_end - clock_start) / CLOCKS_PER_SEC);
    if(pos < vet_size)
        printf("> Found on pos %d.\n", pos);
    else
        printf("> Target not found.\n");

    printf("Ordering the vector for binary search...\n");
    fflush(stdout);

    clock_start = clock();
    qsort(vector, vet_size, sizeof(int), cmpfunc);
    clock_end = clock();

    printf("> Done after %lf seconds.\n", (double)(clock_end - clock_start) / CLOCKS_PER_SEC);

    printf("Binary:\n");

    clock_start = clock();
    pos = binarySearch(vector, vet_size, target);
    clock_end = clock();

    printf("> Done after %lf seconds.\n", (double)(clock_end - clock_start) / CLOCKS_PER_SEC);
    if(pos < vet_size)
        printf("> Found on pos %d.\n", pos);
    else
        printf("> Target not found.\n");

    free(vector);

}

int* createVector (unsigned size) {
    int* vet = (int*)malloc(size * sizeof(int));

    for(int i = 0; i < size; i++)
        vet[i] = rand()%size;

    return vet;

}

// With any vector.
int sequentialSearch (int* vet, int size, int target) {

    for(int i = 0; i < size; i++)
        if(vet[i] == target)
            return i;

    return size;

}

// With a previously sorted vector.
int binarySearch (int* vet, int size, int target) {

    int min, max;
    min = 0;
    max = size - 1;
    
    int current = 0;

    while(vet[current] != target) {

        current = (min + max) / 2;

        if(target > vet[current])
            min = current + 1;
        else if(target < vet[current])
            max = current - 1;

        if(min > max)
            return size;
    } 

    return current;

}