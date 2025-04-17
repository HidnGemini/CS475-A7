#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"
#include <stdbool.h>

// TODO - Safety Algorithm goes here
bool sanityCheck(int numResources, int numProcesses, int* total, int** allocated) {
    int totalAllocated;
    for (int i = 0; i < numResources; i++) {
        totalAllocated = 0;
        for (int j = 0 ; j < numProcesses; j++) {
            totalAllocated += allocated[j][i];
        }
        if (totalAllocated > total[i]) {
            return false;
        }

    }


    // TODO: what does point 2 mean?


    return true;
}