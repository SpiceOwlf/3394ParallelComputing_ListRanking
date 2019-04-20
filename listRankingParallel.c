#include <stdio.h>
#include <stdlib.h>
// #define N 7;
//nvcc doesn't distinguish this define thing
//can use updateResAll to deal with successor array in same block
//TODO: how to sync between different blocks?
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
  // static int r[5] = {0,2,3,4,0};
  //each num represents the successor
  //2 means 1's successor is 2
  //5 means 2's successor is 5
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
__global__ void updateResOneBlock(int *r, int *s){
  //              result, successor, lenth of array
  //__shared__ int  *finalRes;
  //finalRes = r;
  int *q = s;
  int i = threadIdx.x;
  //int distance;
  //try to update one rounds
  if(i < 7 && q[i] != 0 && q[q[i]] != 0){
    r[i] = r[i] + r[q[i]];
    q[i] = q[q[i]];
}
  __syncthreads();

}

__global__ void updateResAll(int *r, int *s, int *q, int n){
  //              result, successor, lenth of array
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
__global__ void updateOnceBetweenBlocks(int *r1, int *r2, int *s1, int *s2, int n){
  //update only one round. Use if to control it;
  int i = threadIdx.x + blockDim.x * blockIdx.x;
  if( i <= n){
    int next = s1[i];
    int nextNext = s1[next];
    int check = (next != nextNext);
    r2[i] = r1[i] + check*r1[next];
    s2[i] = nextNext;

    // if(s1[i] != 0 && s1[s1[i]] != 0){
    //   res2[i] = res1[i] + res1[s1[i]];
    //   s2[i] = s1[s1[i]];
    // }
  }
}
////////////////////////////////////////

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



   return 0;
}
