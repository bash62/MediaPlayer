#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define CHARSET "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789&{[(\\^@)]}=+*!?-,;._"
#define SEED 684351

int main(int argc, char *argv[]) {
   if (argc != 2) {
      printf("Please specify a number of characters you want\n");
      exit(1);
   }

   int n = atoi(argv[1]);
   
   if (n <= 8) {
      printf("Please specify a number of characters greater than 8\n");
      exit(1);
   }

   srand(time(NULL) + n + SEED);
   char password[n];

   for (size_t i = 0;i < n;i++) {
      int key = rand() % (int) (sizeof CHARSET - 1);
      printf("i > %ld, char -> %c\n", i, CHARSET[key]);
      password[i] = CHARSET[key];
   }

   printf("Votre mot de passe : %s\n", password);
   return 0;
}
