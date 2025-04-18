#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"
#include <stdbool.h>

/*
* +----------------------------------+
* |    AWESOME AND COOL FUNCTIONS    |
* +----------------------------------+
*
* (what would this header be? non-helper functions? also hi alex!)
*/

/**
    Performs the first sanity check. Returns true if we've allocated fewer resources
    than we have total. Prints an error and returns false if not.
 */
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

/**
    Performs the second sanity check. Returns true if all threads have fewer (or equal)
    resources currently allocated than they need total.
 */
bool sanityCheck2(int numResources, int numProcesses, int** max, int** allocated) {
    for (int i = 0; i < numProcesses; i++) {
        if (!isGreater(max[i], allocated[i], numResources)) {
            printf("Integrity test failed: allocated resources exceed demand for Thread %d!\n", i);
            return false;
        }
    } 
    return true;
}

/**
    Runs the safety algorithm. Returns a vector containing a 1 if process i can finish
    and 0 if process i cannot finish.
 */
int* runSafety(int* available, int** allocated, int** needed, int numResources, int numProcesses) {
    int* work = deepCopyVec(available, numResources);
    int* finished = malloc(numProcesses*sizeof(int));

    for (int i = 0; i < numProcesses; i++) {
        finished[i] = 0;
    }

    while (inVec(0, finished, numProcesses)) {
        bool workDone = false;
        for (int i = 0; i < numProcesses; i++) {
            if (!finished[i] && isGreater(work, needed[i], numProcesses)) {
                workDone = true;
                addTo(work, allocated[i], numResources);
                finished[i] = 1;
            }
        }
        if (!workDone) {
            break;
        }
    }

    free(work);
    return finished;
}

/**
    Method to call recursive method allSafeOptionsRecursive(). Creates a finish vector
    full of -1s and runs recursive method
 */
void allSafeOptions(int* available, int** allocated, int** needed, int numResources, int numProcesses) {
    int* finish = malloc(numProcesses*sizeof(int));

    for (int i = 0; i < numProcesses; i++) {
        finish[i] = -1;
    }

    allSafeOptionsRecursive(finish, available, allocated, needed, 0, numResources, numProcesses);
    free(finish);
}

/**
    Recursively prints all SAFE combinations given by the safety algorithm
 */
void allSafeOptionsRecursive(int* finished, int* available, int** allocated, int** needed, int depth, int numResources, int numProcesses) {
    if (!inVec(-1, finished, numProcesses)) {
        printf("SAFE: ");
        for (int i = 0; i < numProcesses; i++) {
            printf(" T%d", finished[i]);
        }
        printf("\n");
    }
    int* finishable = getFinishableVectors(available, needed, numResources, numProcesses);
    for (int i = 0; i < numProcesses; i++){
        if (finishable[i] & !inVec(i, finished, numProcesses)) {
            int* subfinished = deepCopyVec(finished, numProcesses);
            subfinished[depth] = i;
            int* subavailable = deepCopyVec(available, numResources);
            addTo(subavailable, allocated[i], numResources);

            allSafeOptionsRecursive(subfinished, subavailable, allocated, needed, depth+1, numResources, numProcesses);

            free(subfinished);
            free(subavailable);
        }
    }
    free(finishable);
}

/*
* +----------------------------------+
* |         HELPER FUNCTIONS         |
* +----------------------------------+
*/

/**
    Creates a deep copy of input vector
 */
int* deepCopyVec(int* toCopy, int toCopySize) {
    int* copy = (int*) malloc(toCopySize*sizeof(int));
    for (int i = 0; i < toCopySize; i++) {
        copy[i] = toCopy[i];
    }
    return copy;
}

/**
    Creates a deep copy of input matrix
 */
int** deepCopyMat(int** toCopy, int rows, int cols) {
    int** copy = (int**) malloc(rows*cols*sizeof(int));
    for (int i = 0; i < rows; i++) {
        copy[i] = deepCopyVec(toCopy[i], rows);
    }
    return copy;
}

/**
    Returns true if all elements in first input vector are greater than (or equal to)
    respective elements in second input vector. Assumes both vectors are the same size.
 */
bool isGreater(int* vector1, int* vector2, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        if (vector1[i] < vector2[i]) {
            return false;
        }
    }
    return true;
}

/**
    Adds the values of addingFrom vector to addedTo vector. Assumes both vectors are 
    the same size.
 */
void addTo(int* addingTo, int* addingFrom, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        addingTo[i] += addingFrom[i];
    }
}

/**
    Subtracts the values of toBeSubtracted vector from subtracting from vector. Assumes
    both vectors are the same size.
 */
void subtractFrom(int* subtractingFrom, int* toBeSubtracted, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        subtractingFrom[i] -= toBeSubtracted[i];
    }
}

/**
    Returns true if toFind is contained within vec and false otherwise.
 */
bool inVec(int toFind, int* vec, int vecSize) {
    for (int i = 0; i < vecSize; i++) {
        if (vec[i] == toFind) {
            return true;
        }
    }
    return false;
}

/**
    Returns a vector containing a 1 if thread i is finishable given input constraints
    and a 0 otherwise.
 */
int* getFinishableVectors(int* available, int** needed, int numResources, int numProcesses) {
    int* finishable = (int*) malloc(numProcesses * sizeof(int));
    for (int i = 0; i < numProcesses; i++) {
        if (isGreater(available, needed[i], numResources)) {
            finishable[i] = 1;
        } else {
            finishable[i] = 0;
        }
    }
    return finishable;
}

/**
    Frees given 2D array.
 */
void free2DArr(int** arr, int rows) {
    for (int i = 0; i < rows; i++) {
        free(arr[i]);
    }
    free(arr);
}

/**
    Prints given vector.
 */
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

/**
    Prints given matrix.
 */
void printMat(int** mat, int rows, int cols) {
    printf("[\n");
    for (int i = 0; i < rows; i++) {
        printVec(mat[i], cols);
    }
    printf("]\n");
}

