#include <stdio.h>
#include <stdlib.h>

int fibo(int n) {
   printf("%d\n", n);
   if (n <= 2) return 1;
   return fibo(n-1) + fibo(n-2);
}

int main(int argc, char *argv[]) {

   if (argc != 2) {
      printf("Please specify N as a number\n");
      exit(1);
   }

   int n = atoi(argv[1]);

   int res = fibo(n);
   printf("Fib -> %d\n", res);
}




