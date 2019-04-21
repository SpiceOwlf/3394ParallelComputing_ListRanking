#include <stdio.h>
#include <stdlib.h>
#include "linearFunctions.h"









int main ()
{
   int arrayLen = 10;
   int *successor = getSuccessor();
   int *predecessor = getPredecessor();
   // printArray(successor,arrayLen);
   int *res;
   res = initRes(successor, arrayLen);
   // printArray(res,arrayLen);
   int *updateResult;
   updateResult = updateRes(res, successor, arrayLen);
   printArray(updateResult,arrayLen);
/////////////////////////////////////////
// independent set here;
//everything in U is 0 initially;
    // int **u = (int**) malloc(sizeof(int*) * arrayLen);
    // for(int i = 0; i < arrayLen; i++){
    //   u[i] = (int*) malloc(sizeof(int*) * 3);
    // }
    // for(int j = 0; j< arrayLen; j++){
    //   for(int k = 0; k< 3; k++){
    //     u[j][k] = 0;
    //   }
    // }

// print func here
    // for(int i = 0; i< 10; i++){
    //   for(int j = 0; j<3; j++){
    //     printf("%d ",(u[i])[j] );
    //   }
    //   printf("\n" );
    // }
// /////////////////////////////////////////
//
// printArray(res, arrayLen);
// printArray(successor, arrayLen);
// printArray(predecessor, arrayLen);
//     int N[arrayLen];
//     for(int m = 0; m< arrayLen;m++){
//       N[m] = 0;
//     }
//
//     N[1] = 1;
//     N[5] = 2;
//     N[6] = 3;
//     for(int l = 1; l< 10;l++){
//     if(N[l] != 0){
//       u[N[l]][0] = l;
//       u[N[l]][1] = successor[l];
//       u[N[l]][2] = res[l];
//     }
//     res[predecessor[l]] = res[predecessor[l]] + res[l];
//     successor[predecessor[l]] = successor[l];
//     predecessor[successor[l]] = predecessor[l];
//   }
//   printf("1111111111111111111\n" );
//   printArray(res, arrayLen);
//   printArray(successor, arrayLen);
//   printArray(predecessor, arrayLen);
//
//     for(int i = 0; i< 10; i++){
//       for(int j = 0; j<3; j++){
//         printf("%d ",(u[i])[j] );
//       }
//       printf("\n" );
//     }
// //res,successor; predecessor





   return 0;
}
