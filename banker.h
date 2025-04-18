#include <stdbool.h>

/* declarations related to banker's algorithm */
bool sanityCheck1(int, int, int*, int**);
bool sanityCheck2(int, int, int**, int**);
int* runSafety(int*, int**, int**, int, int);
void allSafeOptions(int*, int**, int**, int, int);
void allSafeOptionsRecursive(int*, int*, int**, int**, int, int, int);
int* deepCopyVec(int*, int); 
int** deepCopyMat(int**, int, int); // can't overload functions :(
bool isGreater(int*, int*, int);
void addTo(int*, int*, int);
void subtractFrom(int*, int*, int);
bool inVec(int, int*, int);
int* getFinishableVectors(int*, int**, int, int);
void free2DArr(int**, int);
void printVec(int*, int);
void printMat(int**, int, int);