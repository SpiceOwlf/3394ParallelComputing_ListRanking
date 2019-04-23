#include <stdio.h>
#include <stdlib.h>
#include "linearFunctions.h"
void genRandomList(long* a, size_t n, long max, long head)
{
  long x;
  bool exist=0;

  for (long i=0; i<n; i++)
  {
     x= rand() % max;

     //search for x to see whether it already exisits. to be paralized
     if ( x==head ) exist=1;
     for (long j=0; j<i; j++)
     {
       if ( x==a[j] )
       {
          exist=1;
          break;
       }
     }
     if (exist==0)
       a[i]=x;
     else
     {
       i--;
       exist=0;
     }
  }
}
int main ()
{
  long *a;
  genRandomList(a; );
  void genRandomList(long* a, size_t n, long max, long head)



   return 0;
}
