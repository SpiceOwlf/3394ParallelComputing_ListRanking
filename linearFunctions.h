#include <stdio.h>
#include <stdlib.h>

void printArray(int *l, int listLen){
  for(int x=0; x<listLen; x++){
        printf("%d  ",  *(l+x));
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

int *updateRes(int *r, int *s, int len){
  //              result, successor, lenth of array
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
