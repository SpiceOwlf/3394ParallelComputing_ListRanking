#include <stdio.h>
#include <stdlib.h>

void printArray(int *l, int listLen){
  for(int x=0; x<listLen; x++)
    {
        printf("%d  ",  l[x]);

    }
    printf("\n" );
}

// int *getSuccessor(){
//   //0 is a dummy node;
//   // static int r[10] = {0,2,6,1,5,7,8,3,9,0};
//   // static int r[10] = {0,3,0,5,0,7,0,9,0,0};
//   static int r[10] = {0,2,6,1,5,7,8,3,9,9};
//
//
//   // static int r[5] = {0,2,3,4,0};
//   //each num represents the successor
//   //2 means 1's successor is 2
//   //5 means 2's successor is 5
//   return r;
// }
// int *getPredecessor(){
//   static int r[10] = {0,3,1,7,3,4,2,5,6,8};
//   return r;
// }

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



int **create2Darray(int outer){
  int **u = (int**) malloc(sizeof(int*) * outer);
  for(int i = 0; i < outer; i++){
    *(u+i) = (int*) malloc(sizeof(int) * 4);
  }
  for(int j = 0; j< outer; j++){
    for(int k = 0; k< 4; k++){
      *(*(u+j) +k) = 0;

    }
  }
  return u;
}

void copyl1tol2(int *l1, int *l2, int len){
  for(int i = 0; i< len; i++){
    *(l2+i) = *(l1+i);
  }
}
//---------------------------------------
//extract the independent set
__global__ void indepSet(int *r1, int *r2, int *s1, int *s2,int *p1,int *p2, int n, int *u){
//use 1 as prototype, and store data to 2
  int id = threadIdx.x + blockDim.x * blockIdx.x;
  if( id <= n){
    if(id % 2 == 0){
      //ex. id = 4
      r2[id-1] = r1[id-1]+ r1[id];
      p2[s2[id]] = p1[id];
      s2[p2[id]] = s1[id];

//// --------store data-------
      // *(*(u+id)) = 111;
      // *(*(u+id) + 1) =222;
      u[4*id+0] = id;
      u[4*id+1] = p1[id];
      u[4*id+2] = s1[id];
      u[4*id+3] = r1[id];

      // u[id][1] = p1[id];
      // u[id][2] = s1[id];
      // u[id][3] = r1[id];

      r2[id] = 0;
      s2[id] = 0;
      p2[id] = 0;
    }
  }
}
