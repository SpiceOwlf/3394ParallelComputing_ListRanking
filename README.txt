-remember to change the .c file to .cu file
-This project is written in CUDA language

This is the class project of parallel computing. This project implemented the ListRanking algorithm from Joseph JaJa's book,
An Introduction to parallel Algorithms. The chapter for ListRanking Algorithm is 3.1. It is a naive implementation, and can be 
improved in many cases.


MORE TO DO: 
-USE "atomic write" to ensure the sychronization between blocks;

updates on May 23:
This project is done.
1. Some issues: the "independent set method" is not perfect, which will cause bugs. A better way to optimize that will be using    "alternative coloring" method.
2. Atomic writing can sychronize the infomation between blocks in a more efficient way. I used the main function to acchieve that, but it's not ideal.

