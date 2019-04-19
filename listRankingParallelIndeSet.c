#include <stdio.h>
#include <stdlib.h>
// #define N 7;
//nvcc doesn't distinguish this define thing

void printArray(int *l, int listLen){
  for(int x=0; x<listLen; x++)
    {
        printf("%d  ",  l[x]);

    }
    printf("\n" );
}

int *getSuccessor(){
  //0 is a dummy node;
  static int r[10] = {0,2,6,1,5,7,8,3,9,0};
  //each num represents the successor
  //2 means 1's successor is 2
  //5 means 2's successor is 5
  return r;
}
int *getPredecessor(){
  static int r[10] = {0,3,1,7,0,4,2,5,6,8};
  return r;
}

int *initRes(int *l, int len){
  //for array, can only return the pointer of the array
  //static makes returnable outside local function
  //need to use malloc to assign array space
  int *res = (int *)malloc(len * sizeof(int*));
  for (int i = 0; i < len; i++){
     // res[i] = i + 10;
     if(l[i] == 0){
       res[i] = 0;
     }else{
       res[i] = 1;
     }
  }
  return res;
}

////////////////////////////////////////
__global__ void updateResAll(int *r, int *s, int *q, int n){
  //              result, successor, returnable result, lenth of array
  int i = threadIdx.x;
  if (i < n){
    q[i] = s[i];
    __syncthreads();

    //try to update all
    while(q[i] != 0 && q[q[i]] != 0){
      r[i] = r[i] + r[q[i]];
      q[i] = q[q[i]];
      __syncthreads();
    }
  }
}

__global__ void updateIndependentSet(int *s, int *p, int *r, int n, int *u){
            //input: successor, predecessor, result, length, u: 2-d array, store info
            //output: updated s, p, r
      int i = threadIdx.x;




}
////////////////////////////////////////

int main (){
   int arrayLen = 10;
   int *arr = (int *)malloc(arrayLen*sizeof(int));
   int *res = (int *)malloc(arrayLen*sizeof(int));

   arr = getSuccessor();
   res = initRes(arr, arrayLen);
//-------------------------------------------
//update threads in parallel
   int *hostQ = (int *)malloc(arrayLen*sizeof(int));
   int *updateResult = (int *)malloc (arrayLen*sizeof(int));
   int *devRes, *devArr, *devQ;//device result, array;
   cudaMalloc((void **)&devRes, arrayLen*sizeof(int));
   cudaMalloc((void **)&devArr, arrayLen*sizeof(int));
   cudaMalloc((void **)&devQ, arrayLen*sizeof(int));


   cudaMemcpy(devRes, res, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devArr, arr, arrayLen*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devQ, hostQ, arrayLen*sizeof(int),cudaMemcpyHostToDevice);


   updateResAll<<<1,arrayLen>>>(devRes, devArr,devQ,arrayLen);
     //<<<block,thread>>>
   cudaMemcpy(updateResult, devRes,arrayLen*sizeof(int),cudaMemcpyDeviceToHost);
   // printf("22222222 \n");
   printArray(updateResult,arrayLen);
	 // printf("11111111 \n");
   cudaFree(devRes);
   cudaFree(devArr);


   return 0;
}
