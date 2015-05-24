#include <stdio.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


int main(int argc, char **argv){
  
    if(argc != 2){
      printf("usage : %s image.jpg\n",argv[0]);
      return EXIT_FAILURE;
    }
  
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_Surface* image = IMG_Load(argv[1]);
    if(image == NULL) {
        fprintf(stderr, "Impossible de charger l'image %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    
    SDL_FreeSurface(image);
    
    SDL_Quit();

    return EXIT_SUCCESS;
}
