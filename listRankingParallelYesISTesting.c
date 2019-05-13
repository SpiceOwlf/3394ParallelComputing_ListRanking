#include <stdio.h>
#include <stdlib.h>
#include "parallelFunctions.h"
#include "genRandom.h"
#include "timerc.h"
// #define N 7;
//nvcc doesn't distinguish this define thing
//TODO: RANDOMMMMMMM EVERYTHINGGGG!!

////-----------print 2d array----------
//   for(int i = 0; i< arrayLen; i++){
//     for(int j = 0; j<4; j++){
//       printf("%d ",(updateU[4*i+j]) );
//     }
//     printf("\n" );
//   }
////---------------------------------
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

////////////////////////////////////////////////////////////////////

int main (){
   int arrayLen = 10;
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

   // cudaMemcpy(updateResultRes, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   // cudaMemcpy(updateResultSuc, devSuc1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   // cudaMemcpy(updateResultPre, devPre1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   // printArray(updateResultRes,arrayLen);
   // printArray(updateResultSuc,arrayLen);
   // printArray(updateResultPre,arrayLen);



   //-------------------------------------------
   //dealing with independent sets

   indepSetNew<<<div,arrayLen/div>>>(devRes1,devRes2,devSuc1,devSuc2,devPre1,devPre2,arrayLen,devStorage);
  ////2 has the adjusted data



   // ------testing time-----
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

   printArray(updateResultRes,arrayLen);
   printArray(updateResultSuc,arrayLen);
   printArray(updateResultPre,arrayLen);

   cudaMemcpy(updateU, devStorage,arrayLen*4*sizeof(int),cudaMemcpyDeviceToHost);
   //   for(int i = 0; i< arrayLen; i++){
   //     for(int j = 0; j<4; j++){
   //       printf("%d ",(updateU[4*i+j]) );
   //     }
   //     printf("\n" );
   //   }




   cudaFree(devRes1);
   cudaFree(devRes2);
   cudaFree(devSuc1);
   cudaFree(devSuc2);
   cudaFree(devPre1);
   cudaFree(devPre2);


   return 0;
}
