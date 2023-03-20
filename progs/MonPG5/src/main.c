/**                                        
 *   ██╗  ██╗  █████╗   ██╗  ██╗  █████╗  
 *   ██║  ██║ ██╔═══██╗ ██║ ██╔╝ ██╔══██╗ 
 *   ███████║ ██║   ██║ █████╔╝  ███████║ 
 *   ██╔══██║ ██║   ██║ ██╔═██╗  ██╔══██║ 
 *   ██║  ██║ ╚██████╔╝ ██║  ██╗ ██║  ██║ 
 *   ╚═╝  ╚═╝  ╚═════╝  ╚═╝  ╚═╝ ╚═╝  ╚═╝ 
 *                                        
 *  author: hokanosekai
 *  project: pacman
 *  date: 2023-01-12                      
 *  version: 0.0.1                        
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <pthread.h>

#include "window.h"
#include "game.h"
#include "game_state.h"

#define WINDOW_WIDTH 1120
#define WINDOW_HEIGHT 800
#define WINDOW_SCALE 1

int main(int argc, char *argv[])
{
  srand(time(NULL));
  Game *game;

  // Init SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Erreur d'initialisation de SDL : %s\n", SDL_GetError());
    cleanup(NULL, NULL, NULL);
    return EXIT_FAILURE;
  }

  // Init SDL_image
  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "Erreur d'initialisation de SDL_image : %s\n", IMG_GetError());
    cleanup(NULL, NULL, NULL);
    return EXIT_FAILURE;
  }

  // Init SDL_ttf
  if (TTF_Init() < 0) {
    fprintf(stderr, "Erreur d'initialisation de SDL_ttf : %s\n", TTF_GetError());
    cleanup(NULL, NULL, NULL);
    return EXIT_FAILURE;
  }

  // Create game instance
  game = game_create(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_SCALE);
  if (game == NULL) {
    return EXIT_FAILURE;
  }

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, game_run, game);

  // Run game
  pthread_join(thread_id, NULL);

  // Destroy game instance
  game_destroy(game);

  return EXIT_SUCCESS;
}
