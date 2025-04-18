#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"
#include <stdbool.h>

bool sanityCheck1(int numResources, int numProcesses, int* total, int** allocated) {
    int totalAllocated;
    for (int i = 0; i < numResources; i++) {
        totalAllocated = 0;
        for (int j = 0 ; j < numProcesses; j++) {
            totalAllocated += allocated[j][i];
        }
        if (totalAllocated > total[i]) {
            printf("Integrity test failed: allocated resources exceed total resources!\n");
            return false;
        }

    }
    return true;
}

bool sanityCheck2(int numResources, int numProcesses, int** max, int** allocated) {
    for (int i = 0; i < numProcesses; i++) {
        if (!isGreater(max[i], allocated[i], numResources)) {
            printf("Integrity test failed: allocated resources exceed demand for Thread %d\n", i);
            return false;
        }
    } 
    return true;
}

bool isSafe(int* available, int** allocated, int** needed, int numResources, int numProcesses) {
    int* work = deepCopyVec(available, numResources);
    int* finish = malloc(numProcesses*sizeof(int));

    for (int i = 0; i < numProcesses; i++) {
        finish[i] = 0;
    }

    while (inVec(0, finish, numProcesses)) {
        bool workDone = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finish[i] && isGreater(work, needed[i], numProcesses)) {
                printf("finishing Thread %d\n", i);
                workDone = true;
                addTo(work, allocated[i], numResources);
                finish[i] = 1;
            }
        }
        if (!workDone) {
            break;
        }
    }

    printVec(finish, numProcesses);

    free(work);
    free(finish);
    return !(inVec(0, finish, numProcesses));
}





/*
* +----------------------------------+
* |         HELPER FUNCTIONS         |
* +----------------------------------+
*/

int* deepCopyVec(int* toCopy, int toCopySize) {
    int* copy = (int*) malloc(toCopySize*sizeof(int));
    for (int i = 0; i < toCopySize; i++) {
        copy[i] = toCopy[i];
    }
    return copy;
}

int** deepCopyMat(int** toCopy, int rows, int cols) {
    int** copy = (int**) malloc(rows*cols*sizeof(int));
    for (int i = 0; i < rows; i++) {
        copy[i] = deepCopyVec(toCopy[i], rows);
    }
    return copy;
}

bool isGreater(int* vector1, int* vector2, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        if (vector1[i] < vector2[i]) {
            return false;
        }
    }
    return true;
}

void addTo(int* addingTo, int* addedTo, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        addingTo[i] += addedTo[i];
    }
}

void subtractFrom(int* subtractingFrom, int* toBeSubtracted, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        subtractingFrom[i] -= toBeSubtracted[i];
    }
}

bool inVec(int toFind, int* vec, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        if (vec[i] == toFind) {
            return true;
        }
    }
    return false;
}

void printVec(int* vec, int vecSize) {
    printf("[");
    for (int i = 0; i < vecSize; i++) {
        printf("%d", vec[i]);
        if (i < vecSize-1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void printMat(int** mat, int rows, int cols) {
    printf("[\n");
    for (int i = 0; i < rows; i++) {
        printVec(mat[i], cols);
    }
    printf("]\n");
}

