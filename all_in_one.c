#include <stdio.h>
#include <stdlib.h>
#include "linearFunctions.h"
#include "parallelFunctions.h"
#include "genRandom.h"
#include "timerc.h"
__global__ void updateIndepSet(int *r1, int *s1, int *p1, int n, int *u){
  //this function retrive the full set of data
  int id = threadIdx.x + blockDim.x * blockIdx.x;
  if(id<=n && id%2 ==0){
    // printf("%d\n",u[4*id] );
    // printf("11111111111\n" );
    p1[u[4*id]] = u[4*id +1];
    p1[u[4*id+2]] = u[4*id];
    r1[u[4*id]] = u[4*id+3] + r1[u[4*id+2]];
  }
}

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
   int arrayLen = 10;
   float cpu_time;
   int *successor = randlist(arrayLen);
   int *predecessor = generatePredecessor(successor, arrayLen);
   // int *successor = newSuccessor(arrayLen);
   // int *predecessor = newPredessor(arrayLen);
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
// int *arr1 = (int *)malloc(arrayLen*sizeof(int));
// int *arr2 = (int *)malloc(arrayLen*sizeof(int));
// int *res1 = (int *)malloc(arrayLen*sizeof(int));
// int *res2 = (int *)malloc(arrayLen*sizeof(int));
//
// arr1 = successor;
// arr2 = arr1;
// res1 = initRes(arr1, arrayLen);
// res2 = res1;
//
// //-------------------------------------------
// //update threads in parallel
//
// int *updateResultP = (int *)malloc (arrayLen*sizeof(int));
// int *devRes1, *devArr1;//device result, array;
// int *devRes2, *devArr2;//device result, array;
// cudaMalloc((void **)&devRes1, arrayLen*sizeof(int));
// cudaMalloc((void **)&devArr1, arrayLen*sizeof(int));
// cudaMalloc((void **)&devRes2, arrayLen*sizeof(int));
// cudaMalloc((void **)&devArr2, arrayLen*sizeof(int));
//
// cudaMemcpy(devRes1, res1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
// cudaMemcpy(devRes2, res2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
// cudaMemcpy(devArr1, arr1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
// cudaMemcpy(devArr2, arr2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
//
// //------testing time-----
// float parallelTime;
// gstart();
// int counter = 1;
// int div = 2;
// while(counter <= arrayLen/2){
//   //1->2, 2->1
//    //<<<block,thread>>>
//   updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devArr1,devArr2,arrayLen);
//   updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devArr2,devArr1,arrayLen);
//   counter = counter * 2;
// }
// cudaMemcpy(updateResultP, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
// gend(&parallelTime);
// printf("parallelTime is  %f\n", parallelTime);
// // printArray(updateResultP,arrayLen);
// checkSame(updateResult,updateResultP,arrayLen);
//
// cudaFree(devRes1);
// cudaFree(devRes2);
// cudaFree(devArr1);
// cudaFree(devArr2);

/////////////////////////////////////////
int div = 2;
int *suc1 = (int *)malloc(arrayLen*sizeof(int));
int *suc2 = (int *)malloc(arrayLen*sizeof(int));
int *res1 = (int *)malloc(arrayLen*sizeof(int));
int *res2 = (int *)malloc(arrayLen*sizeof(int));
int *pre1 = (int *)malloc(arrayLen*sizeof(int));
int *pre2 = (int *)malloc(arrayLen*sizeof(int));
int *uArray = (int *)malloc(arrayLen*4*sizeof(int));

suc1 = newSuccessor(arrayLen);
copyl1tol2(suc1,suc2,arrayLen);
res1 = initRes(suc1, arrayLen);
copyl1tol2(res1,res2,arrayLen);
pre1 = newPredessor(arrayLen);
copyl1tol2(pre1,pre2,arrayLen);
// printArray(suc2,arrayLen);
// printArray(pre2,arrayLen);
// printArray(res2,arrayLen);


//-------------------------------------------
//update threads in parallel

int *devRes1, *devSuc1;//device result, array;
int *devRes2, *devSuc2;//device result, array;
int *devPre1, *devPre2;//device result, array;
int *devStorage;

cudaMalloc((void **)&devRes1, arrayLen*sizeof(int));
cudaMalloc((void **)&devRes2, arrayLen*sizeof(int));
cudaMalloc((void **)&devSuc1, arrayLen*sizeof(int));
cudaMalloc((void **)&devSuc2, arrayLen*sizeof(int));
cudaMalloc((void **)&devPre1, arrayLen*sizeof(int));
cudaMalloc((void **)&devPre2, arrayLen*sizeof(int));
cudaMalloc((void **)&devStorage, arrayLen*4*sizeof(int));

cudaMemcpy(devRes1, res1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devRes2, res2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devSuc1, suc1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devSuc2, suc2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devPre1, pre1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devPre2, pre2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
cudaMemcpy(devStorage, uArray, arrayLen*4*sizeof(int),cudaMemcpyHostToDevice);

//-------------------------------------------
int *updateResultRes = (int *)malloc (arrayLen*sizeof(int));
int *updateResultSuc = (int *)malloc (arrayLen*sizeof(int));
int *updateResultPre = (int *)malloc (arrayLen*sizeof(int));
int *updateU = (int *)malloc(arrayLen*4*sizeof(int));





//-------------------------------------------
//dealing with independent sets

indepSet<<<div,arrayLen/div>>>(devRes1,devRes2,devSuc1,devSuc2,devPre1,devPre2,arrayLen,devStorage);
float parallelTime;
gstart();
int counter = 1;

while(counter <= arrayLen/2){
  //1->2, 2->1
   //<<<block,thread>>>
  updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devSuc2,devSuc1,arrayLen);
  updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devSuc1,devSuc2,arrayLen);
  counter = counter * 2;
}
//----------------------------------------------
//retrive original data

updateIndepSet<<<div,arrayLen/div>>>(devRes2, devSuc2, devPre2, arrayLen, devStorage);
gend(&parallelTime);
printf("parallelTime is  %f\n", parallelTime);



cudaMemcpy(updateResultRes, devRes2,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
cudaMemcpy(updateResultSuc, devSuc2,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
cudaMemcpy(updateResultPre, devPre2,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);

// printArray(updateResultRes,arrayLen);
// printArray(updateResultSuc,arrayLen);
// printArray(updateResultPre,arrayLen);

cudaMemcpy(updateU, devStorage,arrayLen*4*sizeof(int),cudaMemcpyDeviceToHost);

cudaFree(devRes1);
cudaFree(devRes2);
cudaFree(devSuc1);
cudaFree(devSuc2);
cudaFree(devPre1);
cudaFree(devPre2);

checkSame(updateResult,updateU,arrayLen);

   return 0;
}
