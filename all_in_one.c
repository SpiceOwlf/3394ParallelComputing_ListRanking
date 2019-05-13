#include <stdio.h>
#include <stdlib.h>
#include "linearFunctions.h"
#include "parallelFunctions.h"
#include "genRandom.h"
#include "timerc.h"

int checkSame(int *a, int *b, int len){
  for(int i =0; i< len; i++){
    if(a[i] != b[i]){
      printf("not same\n");
      return 0;
    }
  }
  printf("same\n");
  return 1;
}

int main ()
{
   int arrayLen = 512;
   float cpu_time;
   int *successor = newSuccessor(arrayLen);
   int *predecessor = newPredessor(arrayLen);
   // printArray(successor,arrayLen);
   int *res;
   int *updateResult;
   cstart();
   res = initRes(successor, arrayLen);
   updateResult = updateRes(res, successor, arrayLen);
   cend(&cpu_time);

// printArray(updateResult,arrayLen);
printf("costing time: %f \n",  cpu_time);
printf("---------------------------------\n");
printf("---------------------------------\n");
printf("---------------------------------\n");
printf("---------------------------------\n");
printf("---------------------------------\n");
int *arr1 = (int *)malloc(arrayLen*sizeof(int));
int *arr2 = (int *)malloc(arrayLen*sizeof(int));
int *res1 = (int *)malloc(arrayLen*sizeof(int));
int *res2 = (int *)malloc(arrayLen*sizeof(int));

arr1 = newSuccessor(arrayLen);
arr2 = arr1;
res1 = initRes(arr1, arrayLen);
res2 = res1;

//-------------------------------------------
//update threads in parallel

int *updateResultP = (int *)malloc (arrayLen*sizeof(int));
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

//------testing time-----
float parallelTime;
gstart();
int counter = 1;
int div = 2;
while(counter <= arrayLen/2){
  //1->2, 2->1
   //<<<block,thread>>>
  updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devArr1,devArr2,arrayLen);
  updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devArr2,devArr1,arrayLen);
  counter = counter * 2;
}
cudaMemcpy(updateResultP, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
gend(&parallelTime);
printf("parallelTime is  %f\n", parallelTime);
// printArray(updateResultP,arrayLen);
checkSame(updateResult,updateResultP,arrayLen);

cudaFree(devRes1);
cudaFree(devRes2);
cudaFree(devArr1);
cudaFree(devArr2);



   return 0;
}
