#include <stdbool.h>

/* declarations related to banker's algorithm */
bool sanityCheck1(int, int, int*, int**);
bool sanityCheck2(int, int, int**, int**);
bool isSafe(int*, int**, int**, int, int);
int* deepCopyVec(int*, int); 
int** deepCopyMat(int**, int, int); // can't overload functions :(
bool isGreater(int*, int*, int);
void addTo(int*, int*, int);
void subtractFrom(int*, int*, int);
bool inVec(int, int*, int);
void printVec(int*, int);
void printMat(int**, int, int);