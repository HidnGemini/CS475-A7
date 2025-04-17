#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

int main(int argc, char *argv[])
{
  // TODO: attempt to open scenario file and scan data into allocated structures
  FILE * file = fopen("testfail1.txt", "r");
  int numResources;
  int numProcesses;
  int* totalResources;
  int** maxMatrix;
  int** allocMatrix;

  if (file != NULL) {
    // get number of processes and resources
    fscanf(file, "%d", &numResources);
    fscanf(file, "%d", &numProcesses);
    printf("num resources: %d\nnum processes: %d\n", numResources, numProcesses);

    // allocate arrays
    totalResources = (int*) malloc(sizeof(int)*numResources);
    maxMatrix = (int**) malloc(sizeof(int*)*numProcesses);
    allocMatrix = (int**) malloc(sizeof(int*)*numProcesses);

    // populate totalResources
    for (int i = 0; i < numResources; i++) {
      fscanf(file, "%d", &totalResources[i]);
    }

    // allocate and populate maxMatrix subarrays
    for (int i = 0; i < numProcesses; i++) {
      maxMatrix[i] = (int*) malloc(sizeof(int*)*numResources);
      for (int j = 0; j < numResources; j++) {
        fscanf(file, "%d", &maxMatrix[i][j]);
      }
    }

    // allocate and populate allocMatrix subarrays
    for (int i = 0; i < numProcesses; i++) {
      allocMatrix[i] = (int*) malloc(sizeof(int*)*numResources);
      for (int j = 0; j < numResources; j++) {
        fscanf(file, "%d", &allocMatrix[i][j]);
      }
    }

    if (sanityCheck(numResources, numProcesses, totalResources, allocMatrix)) {

    } else {
      printf("failed sanity check!\n");
    }

  } else {
    printf("file not found! please try again with a valid file!\n");
  }

  // TODO: Run banker's safety algorithm

  return 0;
}