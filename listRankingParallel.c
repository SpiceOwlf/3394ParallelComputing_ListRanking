#include <stdio.h>
#include <stdlib.h>
#include "parallelFunctions.h"
// #define N 7;
//nvcc doesn't distinguish this define thing
//can use updateResAll to deal with successor1 array in same block
//TODO: how to sync between different blocks?

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
   int arrayLen = 10;
   int *successor1 = (int *)malloc(arrayLen*sizeof(int));
   int *successor2 = (int *)malloc(arrayLen*sizeof(int));
   int *res1 = (int *)malloc(arrayLen*sizeof(int));
   int *res2 = (int *)malloc(arrayLen*sizeof(int));
   int *predecessor1 = (int *)malloc(arrayLen*sizeof(int));
   int *predecessor2 = (int *)malloc(arrayLen*sizeof(int));

   successor1 = getSuccessor();
   successor2 = successor1;
   res1 = initRes(successor1, arrayLen);
   res2 = res1;
   predecessor1 = getPredecessor();
   predecessor2 = predecessor1;

   int **u = create2Darray(10);
   //print 2D array
       for(int i = 0; i< 10; i++){
         for(int j = 0; j<4; j++){
           printf("%d ",u[i][j] );
         }
         printf("\n" );
       }
   int head;
   for(int i = 1; i< arrayLen; i++){
     if(predecessor1[i] ==0){
       head = i;

     }
   }
   int i = 1;
   int *newS = (int *)malloc(arrayLen * sizeof(int*));
   int *newP = (int *)malloc(arrayLen * sizeof(int*));
   int *newR = (int *)malloc(arrayLen * sizeof(int*));

   for(int i = 0; i< arrayLen; i++){
     newS[i] = successor1[i];
     newP[i] = predecessor1[i];
     newR[i] = res1[i];
   }
   while(successor1[successor1[head]] != 0){
     u[i][0] = head;
     u[i][1] = successor1[head];
     u[i][2] = predecessor1[head];
     u[i][3] = res1[head];
     newR[predecessor1[head]] = res1[predecessor1[head]] + res1[head];
     newS[predecessor1[head]] = successor1[head];
     newP[successor1[head]] = predecessor1[head];
     // printf("%d\n", head);
     head = successor1[successor1[head]];
     i = i+2;
   }
   // printf("res: " );
   // printArray(newR, arrayLen);
   // printf("successor: " );
   // printArray(newS, arrayLen);
   // printf("predecessor: " );
   // printArray(newP, arrayLen);
   // //print 2D array
   //     for(int i = 0; i< 10; i++){
   //       for(int j = 0; j<4; j++){
   //         printf("%d ",u[i][j] );
   //       }
   //       printf("\n" );
   //     }



  // printArray(res2,arrayLen);
//-------------------------------------------
//update threads in parallel
   // int *hostQ = (int *)malloc(arrayLen*sizeof(int));
   int *updateResult = (int *)malloc (arrayLen*sizeof(int));
   int *devRes1, *devArr1;//device result, array;
   int *devRes2, *devArr2;//device result, array;
   cudaMalloc((void **)&devRes1, arrayLen*sizeof(int));
   cudaMalloc((void **)&devArr1, arrayLen*sizeof(int));
   cudaMalloc((void **)&devRes2, arrayLen*sizeof(int));
   cudaMalloc((void **)&devArr2, arrayLen*sizeof(int));

   cudaMemcpy(devRes1, res1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devRes2, res2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devArr1, successor1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devArr2, successor2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);




   cudaMemcpy(updateResult, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   cudaMemcpy(successor1, devArr1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);

   // printArray(updateResult,arrayLen);
   // printArray(successor1,arrayLen);



//parallel working here.
   int counter = 1;
   int div = 2;
   while(counter <= arrayLen/2){
     //1->2, 2->1
      //<<<block,thread>>>
     updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devArr1,devArr2,arrayLen);
     updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devArr2,devArr1,arrayLen);
     counter = counter * 2;
   }





   cudaFree(devRes1);
   cudaFree(devRes2);
   cudaFree(devArr1);
   cudaFree(devArr2);

   //-------------------------------------------
   //dealing with independent sets


   return 0;
}
