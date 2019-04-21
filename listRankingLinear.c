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
   int *updateResult;
   updateResult = updateRes(res, successor, arrayLen);
   // printArray(updateResult,arrayLen);
/////////////////////////////////////////
// independent set here;
//everything in U is 0 initially;
  int **u = create2Darray(10);
//print 2D array
    for(int i = 0; i< 10; i++){
      for(int j = 0; j<3; j++){
        printf("%d ",u[i][j] );
      }
      printf("\n" );
    }
///////////////////////////////////////
printf("res: " );
printArray(res, arrayLen);
printf("successor: " );
printArray(successor, arrayLen);
printf("predecessor: " );
printArray(predecessor, arrayLen);
int *n_list = setValueForN(arrayLen);
printArray(n_list, arrayLen);

// for(int i = 1; i< 10; i++){
// if(n_list[i] != 0){
//   u[n_list[i]][0] = i;
//   u[n_list[i]][1] = successor[i];
//   u[n_list[i]][2] = res[i];
// }
// res[predecessor[l]] = res[predecessor[l]] + res[l];
// successor[predecessor[l]] = successor[l];
// predecessor[successor[l]] = predecessor[l];
//
// }

  int i = 1;
  u[n_list[i]][0] = i;
  u[n_list[i]][1] = successor[i];
  u[n_list[i]][2] = res[i];

res[predecessor[i]] = res[predecessor[i]] + res[i];
successor[predecessor[i]] = successor[i];
predecessor[successor[i]] = predecessor[i];
printf("1111111111111111111\n" );


for(int i = 0; i< 10; i++){
for(int j = 0; j<3; j++){
  printf("%d ",u[i][j] );
}
printf("\n" );
}
printf("res: " );
printArray(res, arrayLen);
printf("successor: " );
printArray(successor, arrayLen);
printf("predecessor: " );
printArray(predecessor, arrayLen);


i=3;
u[n_list[i]][0] = i;
u[n_list[i]][1] = successor[i];
u[n_list[i]][2] = res[i];

res[predecessor[i]] = res[predecessor[i]] + res[i];
successor[predecessor[i]] = successor[i];
predecessor[successor[i]] = predecessor[i];
  printf("1111111111111111111\n" );


for(int i = 0; i< 10; i++){
  for(int j = 0; j<3; j++){
    printf("%d ",u[i][j] );
  }
  printf("\n" );
}
printf("res: " );
printArray(res, arrayLen);
printf("successor: " );
printArray(successor, arrayLen);
printf("predecessor: " );
printArray(predecessor, arrayLen);



//// res,successor; predecessor





   return 0;
}
