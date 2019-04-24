#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int getSingleRan(int len){

        int num = (rand() %(len)+1);
        return num;
}

int *randlist(int len){
  int *r = (int *)malloc(len * sizeof(int*));
  int next;
  int counter = 0;
  for (int i = 0; i< len; i++){
    *(r+i) = 0;
  }
  int head = getSingleRan(len-1);
  // printf("%d\n", head );
  while(counter < len-2){
    next = getSingleRan(len -1);

    if(next != head && *(r+next) == 0){
      // printf("%d\n", next );
      // printf("lalalalalalalalaal" );
      *(r+head) = next;
      head = next;
      counter++;
    }
  }
return r;
}
int *generatePredecessor(int *suc, int len){
  int *r = (int *)malloc(len * sizeof(int*));
  for(int i = 0; i< len; i++){
    *(r+i) = 0;
  }
  for(int i = 1; i< len; i++){
    int index = *(suc+i);
    if(index != 0){
      *(r+index) = i;
    }
  }

  return r;
}



int *newSuccessor(int len){
  int *r = (int *)malloc(len * sizeof(int*));
  *(r) = 0;
  for (int i = 1; i< len-1; i++){
    *(r+i) = i+1;
  }
  *(r+len-1) = 0;



  return r;
}
int *newPredessor(int len){
  int *r = (int *)malloc(len * sizeof(int*));
  *(r) = 0;
  *(r + 1) = 0;
  for (int i = 2; i< len; i++){
    *(r+i) = i-1;
  }

  return r;
}
