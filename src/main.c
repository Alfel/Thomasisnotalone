#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "sceneFunctions.h"
#include "gameFunctions.h"



int main(int argc, char** argv)
{

  // Gestion de la fenêtre
  float windowWidth  = 1920;
  float windowHeight = 1080;

  if (-1 == SDL_Init(SDL_INIT_VIDEO))
  {
    fprintf(stderr, "Impossible d'initialiser la SDL. Fin du programme.\n");
    return EXIT_FAILURE;
  }

  setVideoMode(windowWidth, windowHeight);
  SDL_WM_SetCaption("Thomas is not alone", NULL);

  SDL_EnableKeyRepeat(100, 100);

  // Variables d'initialisation des personnages
  int nbCharacters = 2;
  Color colors[2] = { setColor(50, 5, 150), setColor(0, 60, 175) };
  Character* availableCharacters[2];
  availableCharacters[0] = createCharacter(1., 5., colors[0], 5., setPosition(20., -10.), setPosition(-20., -10));
  availableCharacters[1] = createCharacter(1., 2., colors[1], 10., setPosition(-20., 10.), setPosition(20., 10));

  // Variables d'initialisation des obstacles
  Obstacle* obstaclesMap[3];
  obstaclesMap[0] = createObstacle(10., 7., setPosition(0, -10));
  obstaclesMap[1] = createObstacle(20., 12., setPosition(-25., -10));
  obstaclesMap[2] = createObstacle(80., 5., setPosition(-50., -20));


  // Variables de sélection du personnage
  int whichCharacter = 0;
  Character* selectedCharacter = availableCharacters[whichCharacter];


  // Variables pour les déplacements
  int characterMoving = 0;
  float xMoving = 0;
  Direction motion;


  int loop = 1;
  while(loop)
  {

    /* temps au début de la boucle */
    Uint32 startTime = SDL_GetTicks();

    /* dessin */
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    moveCharacter(selectedCharacter, characterMoving, motion, &xMoving, obstaclesMap, 3);

    selectACharacter(availableCharacters, &whichCharacter, &selectedCharacter);

    reachFinalPosition(selectedCharacter, availableCharacters, &whichCharacter);

    drawObstacles(obstaclesMap, 3);
    drawCharacter(availableCharacters[0]);
    drawCharacter(availableCharacters[1]);
    drawAvatar(colors[0]);
    drawFinalPosition(availableCharacters[0]);
    drawFinalPosition(availableCharacters[1]);
    
    SDL_GL_SwapBuffers();
    /* ****** */    

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        loop = 0;
        break;
      }
      
      switch(event.type)
      {

        case SDL_VIDEORESIZE:
        windowWidth  = event.resize.w;
        windowHeight = event.resize.h;
        setVideoMode(windowWidth, windowHeight);
        break;

        case SDL_KEYDOWN:

        switch(event.key.keysym.sym)
        {
          case SDLK_LEFT:
          case 'q':
          characterMoving = 1;
          motion = LEFT;
          break;

          case SDLK_RIGHT:
          case 'd':
          characterMoving = 1;
          motion = RIGHT;
          break;

          /*case SDLK_UP:
          case 'z':
          motion = JUMP;*/

          case SDLK_TAB:
          characterMoving = 0;
          whichCharacter = whichCharacter == 1 ? 0 : whichCharacter + 1;
          break;

          case SDLK_ESCAPE: 
          loop = 0;
          break;

          default:
          break;
        }
        break;

        case SDL_KEYUP:
        characterMoving = 0;

        default:
        break;
      }
    }
    
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    if (elapsedTime < FRAMERATE_MILLISECONDS)
      SDL_Delay(FRAMERATE_MILLISECONDS - elapsedTime);
  }
  
  SDL_Quit();
  
  return EXIT_SUCCESS;
}
