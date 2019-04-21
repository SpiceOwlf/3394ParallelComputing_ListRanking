#include <stdio.h>
#include <stdlib.h>
#include "linearFunctions.h"

int main ()
{
   int arrayLen = 10;
   int *successor = getSuccessor();
   int *predecessor = getPredecessor();
   // printArray(successor,arrayLen);
   int *res;
   res = initRes(successor, arrayLen);
   // printArray(res,arrayLen);
   // int *updateResult;
   // updateResult = updateRes(res, successor, arrayLen);
   // printArray(updateResult,arrayLen);
/////////////////////////////////////////
// independent set here;
//everything in U is 0 initially;
  int **u = create2Darray(10);
// //print 2D array
//     for(int i = 0; i< 10; i++){
//       for(int j = 0; j<4; j++){
//         printf("%d ",u[i][j] );
//       }
//       printf("\n" );
//     }
///////////////////////////////////////
//find independent sets
int head;
for(int i = 1; i< arrayLen; i++){
  if(predecessor[i] ==0){
    head = i;
  }
}

int i = 1;
int *newS = (int *)malloc(arrayLen * sizeof(int*));
int *newP = (int *)malloc(arrayLen * sizeof(int*));
int *newR = (int *)malloc(arrayLen * sizeof(int*));

for(int i = 0; i< arrayLen; i++){
  newS[i] = successor[i];
  newP[i] = predecessor[i];
  newR[i] = res[i];
}

while(successor[successor[head]] != 0){
  u[i][0] = head;
  u[i][1] = successor[head];
  u[i][2] = predecessor[head];
  u[i][3] = res[head];
  newR[predecessor[head]] = res[predecessor[head]] + res[head];
  newS[predecessor[head]] = successor[head];
  newP[successor[head]] = predecessor[head];
  // printf("%d\n", head);
  head = successor[successor[head]];
  i = i+2;
}

printf("res: " );
printArray(newR, arrayLen);
printf("successor: " );
printArray(newS, arrayLen);
printf("predecessor: " );
printArray(newP, arrayLen);

//print 2D array
    for(int i = 0; i< 10; i++){
      for(int j = 0; j<4; j++){
        printf("%d ",u[i][j] );
      }
      printf("\n" );
    }
//node id, s,p,r
//////////////////////////////////////////////////////////////////////////////
//put reserved data back
for(int k = 1;k<arrayLen-2; k=k+1){
newR[u[k][2]] = newR[u[k][2]] - newR[u[k][3]];
newS[u[k][2]] = u[k][0];
newP[u[k][1]]=   u[k][0];
}
//print 2D array
printf("---------------------------------\n" );
printf("res: " );
printArray(newR, arrayLen);
printf("successor: " );
printArray(newS, arrayLen);
printf("predecessor: " );
printArray(newP, arrayLen);




   return 0;
}
