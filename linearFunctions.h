#include <stdio.h>
#include <stdlib.h>

void printArray(int *l, int listLen){
  for(int x=0; x<listLen; x++){
        printf("%d  ",  *(l+x));
    }
    printf("\n" );
}
//later we need to set successor and predecessor automatically
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
int *setValueForN(int len){
  // can i pick it randomly?
  int *n  = (int *)malloc(len * sizeof(int*));
  for(int i = 0; i< len;i++){
    n[i] = 0;
  }
  n[1] = 1;
  n[5] = 2;
  n[6] = 3;
  return n;

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
int **create2Darray(int outer){
  int **u = (int**) malloc(sizeof(int*) * outer);
  for(int i = 0; i < outer; i++){
    *(u+i) = (int*) malloc(sizeof(int) * 3);
  }
  for(int j = 0; j< outer; j++){
    for(int k = 0; k< 3; k++){
      *(*(u+j) +k) = 0;

    }
  }
  return u;
}
// void update2D_U(int *u_list, int *n_list){
//   // [arrayLen][3]
//   for(int i = 1; i< 10; i++){
//   if(n_list[i] != 0){
//     u_list[n_list[i]][0] = i;
//     u_list[n_list[i]][1] = i;
//     u_list[n_list[i]][2] = i;
//   }
//
// }


void print2D(int *arr, int outer){
  for(int i = 0; i< outer; i++){
    for(int j = 0; j<3; j++){
      // printf("%d ", arr[i][j] );
      printf("%d ", *(arr + i*3 +j) );
    }
    printf("\n" );
  }
}




int *updateRes(int *r, int *s, int len){
  //              result, successor, lenth of array
  //q and s are seperate
  int *res = r;
  int *q  = (int *)malloc(len * sizeof(int*));
  for(int j = 0; j< len; j++){
    q[j] = s[j];
  }
  //can use q[i] to find the element
  for(int i =0; i< len; i++){
    while(q[i] != 0 && q[q[i]] != 0){
      res[i] = res[i] + res[q[i]];
      q[i] = q[q[i]];
    }
  }
  return res;
}
