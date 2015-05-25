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
  int loop = 1, level = 0;
  float windowInfo[3] = {1920, 1080, 0};  // width, height, ratio
    
  if (-1 == SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "SDL initialization not working. End of program.\n");
    return EXIT_FAILURE;
  }
  
  setVideoMode(windowInfo[0], windowInfo[1], &windowInfo[2]);
  SDL_WM_SetCaption("You are not alone", NULL);

  // Chargement des images
  GLuint textureId = loadImage("img/menu.jpg");
  GLuint texture2 = loadImage("img/pattern.jpg");
  GLuint currentImage = textureId;

  SDL_EnableKeyRepeat(100, 100);
  
  while (loop)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    displayImage(currentImage, 0, windowInfo);
    
    // Le choix du niveau ne peut se faire que si l'écran d'accueil est passé
    if (level)
    {
      glPushMatrix();
        glRotatef(90., 0., 0., 1.);
        displayTriangle(0-level, 0);
      glPopMatrix();
    }

    SDL_GL_SwapBuffers();
        
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          loop = 0;
          break;

        case SDL_VIDEORESIZE:
          windowInfo[0]  = event.resize.w;
          windowInfo[1] = event.resize.h;
          setVideoMode(windowInfo[0], windowInfo[1], &windowInfo[2]);
          break;

        case SDL_KEYDOWN:
          switch(event.key.keysym.sym)
          {
            case SDLK_ESCAPE: 
              loop = 0;
              break;

            case SDLK_RETURN:
              if (!level)
              {
                level = 1;
                currentImage = texture2;
              }
              else
                startGame(level, windowInfo);
              break;

            case SDLK_DOWN:
              if (level)
                level = (level == NB_LEVELS) ? 1 : level + 1;
              break;

            case SDLK_UP:
              if (level)
                level = (level == 1) ? NB_LEVELS : level - 1;
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
    
  glDeleteTextures(1, &textureId);
    
  SDL_Quit();

  return EXIT_SUCCESS;
}
