#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "genRandom.h"

#include "linearFunctions.h"





int main(){
  //use this to get a pair of successor and predecessor
  int len = 10;
  int *suc = randlist(len);
  int *pre = generatePredecessor(suc, len);
  printArray(suc, len);
  printArray(pre, len);

  return 0;
}
