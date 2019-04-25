#include <stdio.h>
#include <stdlib.h>
#include "parallelFunctions.h"
#include "genRandom.h"
// #include "timerc.h"
// #define N 7;
//nvcc doesn't distinguish this define thing
//TODO: get independent lists? algo 3.2?
//TODO: final algorithm, 3.3?
//TODO: maybe better if set the last node's next is itself?
//TODO: make it RANDOM!

// void print2DArray(int *arr, int listLen){
//   for(int i = 0; i< listLen; i++){
//     for(int j = 0; j<3; j++){
//       printf("%d ",(arr[i][j]) );
//     }
//     printf("\n" );
//   }
//
// }
__global__ void indepSet(int *r1, int *r2, int *s1, int *s2,int *p1,int *p2, int n, int **u){
//use 1 as prototype, and store data to 1
  int id = threadIdx.x + blockDim.x * blockIdx.x;
  if( id <= n){
    if(id % 2 == 0){
      *(r2+id-1) = *(r1 + id) + *(r1+id-1);
      *(s2+id-1) = *(s1+id);
      *(p2+id+1) = *(p1+id);
//--------store data-------
      u[id][0] = id;
      u[id][1] = p1[id];
      u[id][2] = s1[id];
      u[id][3] = r1[id];

      *(r2+id) = 0;
      *(s2+id) = 0;
      *(p2+id) = 0;
    }
  }

}
int main (){
   int arrayLen = 20;
   int *suc1 = (int *)malloc(arrayLen*sizeof(int));
   int *suc2 = (int *)malloc(arrayLen*sizeof(int));
   int *res1 = (int *)malloc(arrayLen*sizeof(int));
   int *res2 = (int *)malloc(arrayLen*sizeof(int));
   int *pre1 = (int *)malloc(arrayLen*sizeof(int));
   int *pre2 = (int *)malloc(arrayLen*sizeof(int));

   suc1 = newSuccessor(arrayLen);

   copyl1tol2(suc1, suc2, arrayLen);
   res1 = initRes(suc1, arrayLen);
   copyl1tol2(res1, res2,arrayLen);
   pre1 = newPredessor(arrayLen);
   copyl1tol2(pre1, pre2,arrayLen);

//-------------------------------------------
//update threads in parallel

   int *updateResult = (int *)malloc (arrayLen*sizeof(int));
   int *devRes1, *devSuc1;//device result, array;
   int *devRes2, *devSuc2;//device result, array;
  int *devPre1, *devPre2;//device result, array;
   cudaMalloc((void **)&devRes1, arrayLen*sizeof(int));
   cudaMalloc((void **)&devSuc1, arrayLen*sizeof(int));
   cudaMalloc((void **)&devRes2, arrayLen*sizeof(int));
   cudaMalloc((void **)&devSuc2, arrayLen*sizeof(int));
   cudaMalloc((void **)&devPre1, arrayLen*sizeof(int));
   cudaMalloc((void **)&devPre2, arrayLen*sizeof(int));


   cudaMemcpy(devRes1, res1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devRes2, res2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devSuc1, suc1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devSuc2, suc2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devPre1, pre1, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devPre2, pre2, arrayLen*sizeof(int),cudaMemcpyHostToDevice);

   //-------------------------------------------
   //dealing with independent sets
   // int **uArray = create2Darray(arrayLen);
   // indepSet(devRes1, devRes2, devSuc1, devSuc2,devPre1,devPre2, arrayLen, uArray );
   // __global__ void indepSet(int *r1, int *r2, int *s1, int *s2,int *p1,int *p2, int n, int **u){




   //------testing time usage-----
   // float parallelTime;
   // gstart();
   int counter = 1;
   int div = 2;
   while(counter <= arrayLen/2){
     //1->2, 2->1
      //<<<block,thread>>>
      updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devSuc1,devSuc2,arrayLen);

      updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devSuc2,devSuc1,arrayLen);
      // updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devSuc1,devSuc2,arrayLen);

     // updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes1, devRes2,devSuc1,devSuc2,arrayLen);
     // updateOnceBetweenBlocks<<<div,arrayLen/div>>>(devRes2, devRes1,devSuc2,devSuc1,arrayLen);
     counter = counter * 2;
   }
   cudaMemcpy(updateResult, devRes1,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);

   // cudaMemcpy(updateResult, devRes2,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   // gend(&parallelTime);
   // printf("parallelTime is  %f\n", parallelTime);
   printArray(updateResult,arrayLen);


   cudaFree(devRes1);
   cudaFree(devRes2);
   cudaFree(devSuc1);
   cudaFree(devSuc2);




   return 0;
}
