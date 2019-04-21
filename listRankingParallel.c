#include <stdio.h>
#include <stdlib.h>
#include "parallelFunctions.h"
// #define N 7;
//nvcc doesn't distinguish this define thing
//can use updateResAll to deal with successor array in same block
//TODO: how to sync between different blocks?

void print2DArray(int *arr, int listLen){
  for(int i = 0; i< listLen; i++){
    for(int j = 0; j<3; j++){
      printf("%d ",(arr[i][j]) );
    }
    printf("\n" );
  }

}



int main (){
   int arrayLen = 10;
   int *arr1 = (int *)malloc(arrayLen*sizeof(int));
   int *arr2 = (int *)malloc(arrayLen*sizeof(int));
   int *res1 = (int *)malloc(arrayLen*sizeof(int));
   int *res2 = (int *)malloc(arrayLen*sizeof(int));

   arr1 = getSuccessor();
   arr2 = arr1;
   res1 = initRes(arr1, arrayLen);
   res2 = res1;

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
   cudaMemcpy(devArr1, arr1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devArr2, arr2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);

   int counter = 1;
   int div = 2;
   while(counter <= arrayLen/2){
     //1->2, 2->1
      //<<<block,thread>>>
     updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devArr1,devArr2,arrayLen);
     updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devArr2,devArr1,arrayLen);
     counter = counter * 2;
   }



   cudaMemcpy(updateResult, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   printf("22222222 \n");
   printArray(updateResult,arrayLen);
	 printf("11111111 \n");

   cudaFree(devRes1);
   cudaFree(devRes2);
   cudaFree(devArr1);
   cudaFree(devArr2);

   //-------------------------------------------
   //dealing with independent sets
 //   int N[arrayLen];
 //   for(int m = 0; m< arrayLen;m++){
 //     N[m] = 0;
 //   }
 //   int **u = (int**) malloc(sizeof(int*) * arrayLen);
 //   for(int i = 0; i < arrayLen; i++){
 //     u[i] = (int*) malloc(sizeof(int*) * 3);
 //   }
 //   for(int j = 0; j< arrayLen; j++){
 //     for(int k = 0; k< 3; k++){
 //       u[j][k] = 0;
 //     }
 //   }
 //
 //   N[1] = 1;
 //   N[5] = 2;
 //   N[6] = 3;
 //   for(int l = 1; l< 10;l++){
 //   if(N[l] != 0){
 //     u[N[l]][0] = l;
 //     u[N[l]][1] = l;
 //     u[N[l]][2] = l;
 //   }
 // }
 // print2DArray(*u, arrayLen);
 // for(int i = 0; i< 10; i++){
 //   for(int j = 0; j<3; j++){
 //     printf("%d ",(u[i])[j] );
 //   }
 //   printf("\n" );
 // }
   //-------------------------------------------


   return 0;
}
