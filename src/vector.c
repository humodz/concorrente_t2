/**
 * Bruno Henrique Rasteiro 9292910
 * Hugo Moraes Dzin 8532186
 * Luiz Eduardo Dorici 4165850
 * Matheus Gomes da Silva Horta 8532321
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"


Vector Vector_read(FILE *fp) {
    Vector vec;
    int capacity = 1000;

    vec.n = 0;
    vec.data = malloc(sizeof(float[capacity]));
    assert(vec.data != NULL);

    float num;

    fscanf(fp, " %f", &num);
    while( ! feof(fp)) {
        // Double the buffer size every time capacity is exceeded
        if(vec.n == capacity) {
            capacity *= 2;
            vec.data = realloc(vec.data, sizeof(float[capacity]));
            assert(vec.data != NULL);
        }

        vec.data[vec.n] = num;
        vec.n += 1;

        fscanf(fp, " %f", &num);
    }

    // Shrink capacity to the correct size
    if(vec.n < capacity) {
        vec.data = realloc(vec.data, sizeof(float[vec.n]));
    }

    vec.indices = malloc(sizeof(int[vec.n]));
    assert(vec.indices != NULL);

    #pragma omp parallel for
    for(int i = 0; i < vec.n; i++) {
        vec.indices[i] = i;
    }

    return vec;
}


void Vector_free(Vector *vec) {
    free(vec->data);
    free(vec->indices);
    vec->data = NULL;
    vec->indices = NULL;
}


// Prints the elements in correct order, regardless of swaps
void Vector_print(Vector *vec, FILE *fp) {
    for(int i = 0; i < vec->n; i++) {
        int pos = vec->indices[i];
        fprintf(fp, "%f\n", vec->data[pos]);
    }
}


// Keeps track of which rows were swapped, so that the
// vector can be printed in correct order
void Vector_swap_rows(Vector *vec, int i, int j) {
    float f = vec->data[i];
    vec->data[i] = vec->data[j];
    vec->data[j] = f;

    int s = vec->indices[i];
    vec->indices[i] = vec->indices[j];
    vec->indices[j] = s;
}
