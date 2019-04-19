#include <stdio.h>
#include "timerc.h"

__global__ void vectorSumBetter(int *input, int size, int *output){
	int abs_thread_idx = blockIdx.x * blockDim.x + threadIdx.x;\
	int numElementsPerBlock = size/gridDim.x;
	for(int s = 1; s<=numElementsPerBlock/2; s=s*2){
	if( abs_thread_idx < numElementsPerBlock/(2*s)){
		input[abs_thread_idx*2*s+blockIdx.x*numElementsPerBlock] += input[abs_thread_idx*2*s+s+numElementsPerBlock*blockIdx.x];

	}
	__syncthreads();
	}
	output[blockIdx.x] = input[blockIdx.x*numElementsPerBlock];
}

__global__ void vector_sum(int *input, int size, int *output){
	int abs_thread_idx = blockIdx.x * blockDim.x + threadIdx.x;
	int numElementsPerThread = size / (blockDim.x * gridDim.x);
	int startPos = abs_thread_idx * numElementsPerThread;
	int localTotal = 0;

	for(int i = 0; i < numElementsPerThread; i++){
		localTotal = localTotal + input[i + startPos];
	}

	__shared__ int totals[2048];
	totals[abs_thread_idx] = localTotal;
	__syncthreads();

	if(abs_thread_idx == 0){

		for(int i = 1; i < blockDim.x * gridDim.x; i++){
			localTotal = localTotal + totals[i];
		}
		output[0] = localTotal;
	}

}

int main(){
	int numElements = 1024 * 1024;
	int *hostInput = (int *) malloc(numElements * sizeof(int));
	for(int i = 0; i < numElements; i++){
		hostInput[i] = 1;
	}
	int *deviceInput;
	int *deviceOutput;
	int hostOutput;

	cudaMalloc((void **) &deviceInput, numElements * sizeof(int));
	cudaMalloc((void **) &deviceOutput, 1 * sizeof(int));

	cudaMemcpy(deviceInput, hostInput, numElements* sizeof(int),cudaMemcpyHostToDevice);
	vector_sum<<<1,1024>>>(deviceInput,numElements,deviceOutput);

	cudaMemcpy(&hostOutput, deviceOutput, 1 * sizeof(int),cudaMemcpyDeviceToHost);
	printf("Total = %d\n", hostOutput);

	vectorSumBetter<<<1,1024>>>(deviceInput, 2048);
	cudaMemcpy(&hostOutput, deviceInput, 1 * sizeof(int),cudaMemcpyDeviceToHost);
	printf("Total = %d\n", hostOutput);



}
