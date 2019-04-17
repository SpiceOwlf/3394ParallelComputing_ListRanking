#include <stdio.h>
#include "timerc.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10

__global__ void add(int *a, int *b, int *c){
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if( i < N){
    c[i] = a[i] + b[i];
  }
}


int main()
{
  int a[N], b[N], c[N];
  int *dev_a, *dev_b, *dev_c;
  int size = N * sizeof(int);
  cudaMalloc((void **) &dev_a, size);
  cudaMalloc((void **) &dev_b, size);
  cudaMalloc((void **) &dev_c, size);
  for (int i = 0; i< N; i++){
    a[i] = i;
    b[i] = i;
  }
  cudaMemcpy(dev_a, a, size, cudaMemcpyHostToDevice);
  //dev_a <- a
  cudaMemcpy(dev_b, b, size, cudaMemcpyHostToDevice);
  cudaMemcpy(dev_c, c, size, cudaMemcpyHostToDevice);
  add<<<10,1>>>(dev_a,dev_b,dev_c);
  //play on cpu
  cudaMemcpy( c,dev_c ,size,cudaMemcpyDeviceToHost);
  for(int i = 0; i< N; i++){
    printf("%d\n",c[i] );
  }
  cudaFree(dev_a);
  cudaFree(dev_b);
  cudaFree(dev_c);
  //<<<block,thread>>>


    // int a[N], b[N], c[N];
    // int *deva, *devb, *devc;
    // //在GPU上分配内存
    // cudaMalloc((void **)&deva, N*sizeof(int));
    // cudaMalloc((void **)&devb, N*sizeof(int));
    // cudaMalloc((void **)&devc, N*sizeof(int));
    //
    // //在CPU上为数组赋值
    // for (int i = 0; i < N; i++)
    // {
    //     a[i] = -i;
    //     b[i] = i*i;
    // }
    // //将数组a和b传到GPU
    // cudaMemcpy(deva, a, N*sizeof(int), cudaMemcpyHostToDevice);
    // cudaMemcpy(devb, b, N*sizeof(int), cudaMemcpyHostToDevice);
    // cudaMemcpy(devc, c, N*sizeof(int), cudaMemcpyHostToDevice);
    // add <<<N, 1 >>>(deva, devb, devc);
    //
    // //将数组c从GPU传到CPU
    // cudaMemcpy(c, devc, N*sizeof(int), cudaMemcpyDeviceToHost);
    // for (int i = 0; i < N; i++)
    // {
    //     printf("%d+%d=%d\n", a[i], b[i], c[i]);
    // }
    // cudaFree(deva);
    // cudaFree(devb);
    // cudaFree(devc);
    return 0;
}
