#include <stdio.h>
#include <stdlib.h>
#include "parallelFunctions.h"
// #include "genRandom.h"
#include "timerc.h"
// #define N 7;
//nvcc doesn't distinguish this define thing
//TODO: get independent lists? algo 3.2?
//TODO: final algorithm, 3.3?

// void print2DArray(int *arr, int listLen){
//   for(int i = 0; i< listLen; i++){
//     for(int j = 0; j<3; j++){
//       printf("%d ",(arr[i][j]) );
//     }
//     printf("\n" );
//   }
//
// }



int main (){
   int arrayLen = 1024*1024;
  float *parallelTime = wrapup(arrayLen);

   printf("parallelTime is  %f\n", parallelTime);
   // printArray(updateResult,arrayLen);



   //-------------------------------------------
   //dealing with independent sets


   return 0;
}
