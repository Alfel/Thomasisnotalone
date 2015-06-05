//===============================================
// main.c
//-----------------------------------------------
// Démarrage du jeu (choix du niveau)
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "windowFunctions.h"
#include "settingFunctions.h"
#include "displayFunctions.h"
#include "gameFunctions.h"
#include "playingFunctions.h"

#define NB_LEVELS 3


int main(int argc, char** argv)
{
  SDL_Event event;
  int level = 0;
  
  if (-1 == SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "SDL initialization not working. End of program.\n");
    return EXIT_FAILURE;
  }

  Window window = setWindow();

  setVideoMode(&window);
  SDL_WM_SetCaption("Potter is not alone", NULL);
  glClearColor(17./256, 22./256, 37./256, 1.);

  // Chargement des images
  GLuint welcomeScreen = loadImage("img/welcome.jpg");
  GLuint levelsScreen = loadImage("img/levels.jpg");
  GLuint currentImage = welcomeScreen;

  SDL_EnableKeyRepeat(100, 100);
  
  while (window.loop == TRUE)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    displayImage(currentImage, FALSE, window);
    
    // Le choix du niveau ne peut se faire que si l'écran d'accueil est passé
    if (level != 0)
    {
      // Affichage triangle
      glPushMatrix();
        glRotatef(90., 0., 0., 1.);
        displayTriangle(5-level*3.67, 7.5);
      glPopMatrix();
    }

    SDL_GL_SwapBuffers();
        
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          window.loop = FALSE;
          break;

        case SDL_VIDEORESIZE:
          window.width  = (event.resize.w < 800) ? 800 : event.resize.w;
          window.height = (event.resize.h < 600) ? 600  : event.resize.h;
          setVideoMode(&window);
          break;

        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
          {
            case SDLK_ESCAPE:
              window.loop = FALSE;
              break;

            case 'f':
              window.fullScreen = (window.fullScreen == TRUE) ? FALSE : TRUE;
              setVideoMode(&window);
              break;

            case SDLK_RETURN:
              if (level == 0)
              {
                level = 1;
                glDeleteTextures(1, &welcomeScreen);
                currentImage = levelsScreen;
              }
              else
                startGame(level, &window);
              break;

            case SDLK_UP:
            case 'z':
              if (level != 0)
                level = (level == 1) ? NB_LEVELS : level - 1;
              break;

            case SDLK_DOWN:
            case 's':
              if (level != 0)
                level = (level == NB_LEVELS) ? 1 : level + 1;
              break;

            default:
              break;
          }
          break;

        default:
          break;
      }
    }
  }
  
  glDeleteTextures(1, &levelsScreen);
    
  SDL_Quit();

  return EXIT_SUCCESS;
}
