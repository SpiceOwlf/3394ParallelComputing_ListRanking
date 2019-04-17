// 提供了cudaThreadSynchronize()
// 来同步同一个block的thread以保证在进行下一步处理之前，
// 所有thread都到达某个时间点。

// CUDA程序构架分为两部分：Host和Device。一般而言，
// Host指的是CPU，Device指的是GPU

// device表示从GPU调用，在GPU上执行
// global表示从CPU调用，在GPU上执行，也称之为kernel函数
// host表示在CPU上调用，在CPU上执行


//grid->block->thread
#include <stdio.h>
#include "timerc.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#define N 10

__global__ void add(int *a, int *b, int *c){
  int i = blockIdx.x * blockDim.x + threadIdx.x;

    c[i] = a[i] + b[i];

}


main(){
int a[N], b[N], c[N];
int *deva, *devb, *devc;
//在GPU上分配内存
cudaMalloc((void **)&deva, N*sizeof(int));
cudaMalloc((void **)&devb, N*sizeof(int));
cudaMalloc((void **)&devc, N*sizeof(int));

//在CPU上为数组赋值
for (int i = 0; i < N; i++)
{
    a[i] = -i;
    b[i] = i*i;
}
//将数组a和b传到GPU
cudaMemcpy(deva, a, N*sizeof(int), cudaMemcpyHostToDevice);
cudaMemcpy(devb, b, N*sizeof(int), cudaMemcpyHostToDevice);
cudaMemcpy(devc, c, N*sizeof(int), cudaMemcpyHostToDevice);
add <<<N, 1 >>>(deva, devb, devc);

//将数组c从GPU传到CPU
cudaMemcpy(c, devc, N*sizeof(int), cudaMemcpyDeviceToHost);
for (int i = 0; i < N; i++)
{
    printf("%d+%d=%d\n", a[i], b[i], c[i]);
}
cudaFree(deva);
cudaFree(devb);
cudaFree(devc);
return 0;}
