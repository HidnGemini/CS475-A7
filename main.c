#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "banker.h"

int main(int argc, char *argv[])
{
  // check args
  if (argc != 2) {  
    printf("ERROR: Bad Arguments! Please try again with correct usage: ./bankers [file]\n");
    return 1;
  }

  // read file and check if it read properly
  FILE * file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("ERROR: File not found! Please try again with a valid file!\n");
    return 1;
  }

  // intialize structures
  int numResources;
  int numProcesses;
  int* totalResources;
  int** maxMatrix;
  int** allocMatrix;

  // get number of processes and resources from file
  fscanf(file, "%d", &numResources);
  fscanf(file, "%d", &numProcesses);

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

  // perform both sanity checks
  if (!(sanityCheck1(numResources, numProcesses, totalResources, allocMatrix) && \
  sanityCheck2(numResources, numProcesses, maxMatrix, allocMatrix))) {
    // free everything and return error 1 if sanity checks fail
    free(totalResources);
    free2DArr(maxMatrix, numProcesses);
    free2DArr(allocMatrix, numProcesses);
    fclose(file);
    return 1;
  } 

  // create available vector
  int* available = deepCopyVec(totalResources, numResources);
  for (int i = 0; i < numProcesses; i++) {
    subtractFrom(available, allocMatrix[i], numResources);
  }

  // create needed matrix
  int** needed = deepCopyMat(maxMatrix, numProcesses, numResources);
  for (int i = 0; i < numProcesses; i++) {
    subtractFrom(needed[i], allocMatrix[i], numResources);
  }

  // run safety algorithm and get its finished matrix
  int* finished = runSafety(available, allocMatrix, needed, numResources, numProcesses);

  // did we finish properly? (no 0s in finished vector)
  if (!inVec(0, finished, numProcesses)) {
    // find all safe combinations recursively
    allSafeOptions(available, allocMatrix, needed, numResources, numProcesses);
  } else {
    // print out threads that couldn't complete
    printf("UNSAFE: ");
    for (int i = 0; i < numProcesses; i++) {
      if (finished[i] == 0) {
        printf(" T%d", i);
      }
    }
    printf(" can't finish\n");
  }

  // free safety algorithm inputs and outputs
  free(available);
  free2DArr(needed, numProcesses);
  free(finished);

  // free everything
  free(totalResources);
  free2DArr(maxMatrix, numProcesses);
  free2DArr(allocMatrix, numProcesses);
  fclose(file);
  
  return 0;
}