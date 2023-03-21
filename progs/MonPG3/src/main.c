#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "pong.h"

#define WINDOW_WIDTH 1120
#define WINDOW_HEIGHT 800
#define WINDOW_SCALE 1

int main() {
   srand(time(NULL));
   Pong *app;

   // Initialize SDL
   if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
      cleanup(NULL, NULL, NULL);
      return EXIT_FAILURE;
   }

   // Initialize SDL Image
   if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
      cleanup(NULL, NULL, NULL);
      return EXIT_FAILURE;
   }

   // Initialize SDL TTF
   if (TTF_Init() == -1) {
      fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
      cleanup(NULL, NULL, NULL);
      return EXIT_FAILURE;
   }

   // Initialize MediaPlayer App instance
   app = pong_create(WINDOW_WIDTH, WINDOW_HEIGHT);
   if (app == NULL) {
      return EXIT_FAILURE;
   }

   // Start the app
   pong_run(app);

   // Destroy the app instance at the end
   pong_destroy(app);

   return EXIT_SUCCESS;
}
