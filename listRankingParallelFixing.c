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
int *getArray(){
  //0 is a dummy node;
  static int r[7] = {0,2,5,4,0,6,3};
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
int *updateRes(int *r, int *s, int len){
  //           result, successor, lenth of array
  int *res = r;
  int *q = s;
  //can use q[i] to find the element
  for(int i =0; i< len; i++){
    while(q[i] != 0 && q[q[i]] != 0){
      res[i] = res[i] + res[q[i]];
      q[i] = q[q[i]];
    }
  }
  return res;
}
////////////////////////////////////////
__global__ void updateResOneBlock(int *r, int *s){
  //              result, successor, lenth of array
  int *q = s;
  int i = threadIdx.x;
  //try to update one rounds
  if(i < 7 && q[i] != 0 && q[q[i]] != 0){
    r[i] = r[i] + r[q[i]];
    q[i] = q[q[i]];
}
  __syncthreads();
}
////////////////////////////////////////
__global__ void updateResAll(int *r, int *s, int *q, int n){
  //              result, successor, lenth of array
  //s changes here bc q points s
  //create space for q
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
////////////////////////////////////////
__global__ void test(int *r, int *s){
  int i = threadIdx.x;
  __shared__ int sRes[7];
  if(i != 0){
    sRes[i] = r[i] + i ;
  }
  __syncthreads();
  for(int i = 0; i< 7; i++){
    r[i] = sRes[i];
  }

}

int main (){
   int arrayLen = 7;
   int *arr = (int *)malloc(7*sizeof(int));
   int *res = (int *)malloc(7*sizeof(int));

   arr = getArray();
   res = initRes(arr, arrayLen);
  // printArray(arr,7);
  // printArray(res,arrayLen);
//-------------------------------------------
//update threads in parallel
   int *updateResult = (int *)malloc(7*sizeof(int));;
   int *devRes, *devArr;//device result, array;
   cudaMalloc((void **)&devRes, 7*sizeof(int));
   cudaMalloc((void **)&devArr, 7*sizeof(int));


   cudaMemcpy(devRes, res, 7*sizeof(int),cudaMemcpyHostToDevice);
   cudaMemcpy(devArr, arr, 7*sizeof(int),cudaMemcpyHostToDevice);

   updateResAll<<<1,7>>>(devRes, devArr);
    // test<<<1,7>>>(devRes, devArr);

     //<<<block,thread>>>
   cudaMemcpy(updateResult, devRes,7*sizeof(int),cudaMemcpyDeviceToHost);
   printf("22222222 \n");
   printArray(updateResult,arrayLen);
	 printf("11111111 \n");
   //seg here
   cudaFree(devRes);
   cudaFree(devArr);


   return 0;
}
