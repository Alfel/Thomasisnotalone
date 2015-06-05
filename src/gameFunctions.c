//===============================================
// gameFunctions.c
//-----------------------------------------------
// Fonctions d'initialisation, démarrage et affichage du jeu lui-même
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "gameFunctions.h"


//===============================================
// Charger et afficher la scène de jeu
//===============================================
void loadGame(Game* game, float ratio, Bool xMoving, Bool* yMoving, Bool* loop)
{
  int i;

  glPushMatrix();

    moveCamera(&game->cameraPosition, game->selectedCharacter);
    glTranslatef(game->cameraPosition.x, game->cameraPosition.y, 0.);

    moveCharacters(game->availableCharacters, game->obstaclesMap, game->nbCharacters, game->nbObstacles, game->whichCharacter, yMoving);
    selectACharacter(game->availableCharacters, game->whichCharacter, &game->selectedCharacter, game->nbCharacters, loop);
    reachFinalPosition(game->selectedCharacter, game->availableCharacters, &game->whichCharacter, game->nbCharacters);

    // Affichage des obstacles
    glColor3ub(game->obstaclesColor.red, game->obstaclesColor.green, game->obstaclesColor.blue);
    for (i = 0; i < game->nbObstacles; ++i)
      displayObstacle(game->obstaclesMap[i]);

    // Affichage des personnages et de leur position finale
    for (i = 0; i < game->nbCharacters; ++i)
    {
      displayCharacter(game->availableCharacters[i]);
      displayFinalPosition(game->availableCharacters[i]);
    }
    
  glPopMatrix();

  // Affichage des petits avatars des personnages en bas à droite
  for (i = 0; i < game->nbCharacters; ++i)
    displayAvatar(game->charactersColor[i], i, ratio);
  selectAvatar(game->whichCharacter, ratio);
}


//===============================================
// Démarrer le jeu en fonction du niveau choisi
//===============================================
void startGame(int level, Window* window)
{
  Game game;
  SDL_Event event;
  Bool loop = TRUE, controls = FALSE, xMoving = FALSE, yMoving = FALSE, completed = FALSE;

  setGame(&game, level);
  GLuint controlsScreen = loadImage("img/controls.jpg");
  GLuint completedScreen = loadImage("img/completed.jpg");
  GLuint pattern = loadImage(game.pattern);

  while (loop == TRUE)
  {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    if (controls == FALSE)
      displayImage(controlsScreen, FALSE, *window);
    else if (completed == TRUE)
      displayImage(completedScreen, FALSE, *window);
    else
    {
      displayImage(pattern, TRUE, *window);
      loadGame(&game, window->ratio, xMoving, &yMoving, &completed);
    }
    
    SDL_GL_SwapBuffers();
        
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          window->loop = FALSE;
          return;

        case SDL_VIDEORESIZE:
          window->width  = (event.resize.w < 800) ? 800 : event.resize.w;
          window->height = (event.resize.h < 600) ? 600  : event.resize.h;
          setVideoMode(window);
          break;

        case SDL_KEYDOWN:

          switch(event.key.keysym.sym)
          {
            case SDLK_ESCAPE:
              loop = FALSE;
              break;

            case 'f':
              window->fullScreen = (window->fullScreen) ? FALSE : TRUE;
              setVideoMode(window);
              break;

            case SDLK_RETURN:
              if (controls == FALSE)
              {
                controls = TRUE;
                glDeleteTextures(1, &controlsScreen);
              }
              else if (completed == TRUE)
              {
                loop = FALSE;
              }
              break;

            case SDLK_LEFT:
            case 'q':
              if (controls == TRUE)
              {
                xMoving = TRUE;
                moveLeft(game.selectedCharacter, xMoving);
              }
            break;

            case SDLK_RIGHT:
            case 'd':
              if (controls == TRUE)
              {
                xMoving = TRUE;
                moveRight(game.selectedCharacter, xMoving);
              }
            break;

            case SDLK_UP:
            case SDLK_SPACE:
            case 'z':
              if (controls == TRUE)
              {
                yMoving = TRUE;
                jump(game.selectedCharacter, yMoving);
              }
            break;

            case SDLK_TAB:
              if (controls == TRUE)
              {
                xMoving = FALSE;
                yMoving = FALSE;
                game.whichCharacter = (game.whichCharacter == game.nbCharacters - 1) ? 0 : game.whichCharacter + 1;
              }
            break;

            default:
            break;
          }
          break;

        case SDL_KEYUP:
          xMoving = FALSE;
          yMoving = FALSE;
          game.selectedCharacter->xSpeed = 0;
          break;

        default:
          break;
      }
    }
  }

  glDeleteTextures(1, &controlsScreen);

}


//===============================================
// Initialiser la scène de jeu selon le fichier correspondant au niveau choisi
//===============================================
void setGame(Game* newGame, int level)
{
  int i;

  // Correspondance niveau <-> fichier correspondant
  char filePath[15] = {0};  
  sprintf(filePath, "lvl/level%d.lvl", level);

  // Ouverture du fichier (r pour read only)
  FILE* file = fopen(filePath, "r");
  if (!file)
  {
    fprintf(stderr, "File %s does not exist. End of program.\n", filePath);
    exit(EXIT_FAILURE);
  }
    

  // Récupération du nombre de personnages & allocation des tableaux en dépendant
  fscanf(file, "%d", &newGame->nbCharacters);
  newGame->availableCharacters = calloc(newGame->nbCharacters, sizeof(Character*));
  if (!newGame->availableCharacters)
    exit(EXIT_FAILURE);
  newGame->charactersColor = calloc(newGame->nbCharacters, sizeof(Color));
  if (!newGame->charactersColor)
    exit(EXIT_FAILURE);

  // Initialisation des personnages & stockage de leur couleur
  for (i = 0; i < newGame->nbCharacters; ++i)
  {
    float width, height, jumpPower, x, y, finalX, finalY;
    int r, g, b;
    fscanf(file, "%f %f %d %d %d %f %f %f %f %f", &width, &height, &r, &g, &b, &jumpPower, &x, &y, &finalX, &finalY);
    newGame->availableCharacters[i] = setCharacter(width, height, setColor(r, g, b), jumpPower, setPosition(x, y), setPosition(finalX, finalY));
    newGame->charactersColor[i] = setColor(r, g, b);
  }

  // Récupération du nombre d'obstacles (+ couleur) & allocation du tableau en dépendant
  fscanf(file, "%d", &newGame->nbObstacles);
  int r, g, b;
  fscanf(file, "%d %d %d", &r, &g, &b);
  newGame->obstaclesColor = setColor(r, g, b);
  newGame->obstaclesMap = calloc(newGame->nbObstacles, sizeof(Obstacle*));
  if (!newGame->obstaclesMap)
    exit(EXIT_FAILURE);
  
  // Initialisation des obstacles
  for (i = 0; i < newGame->nbObstacles; ++i)
  {
    float width, height, x, y;
    fscanf(file, "%f %f %f %f", &width, &height, &x, &y);
    newGame->obstaclesMap[i] = setObstacle(width, height, setPosition(x, y));
  }

  // Fermeture du fichier
  fclose(file);

  //char pattern[15] = {0};  
  sprintf(newGame->pattern, "img/pattern%d.jpg", level);

  newGame->whichCharacter = 0;
  newGame->selectedCharacter = newGame->availableCharacters[newGame->whichCharacter];

  // Position caméra
  newGame->cameraPosition = setPosition(-newGame->selectedCharacter->position.x, -newGame->selectedCharacter->position.y);
}


//===============================================
// Bouger la caméra de manière douce et agréable, en toute sérénité
// (Bon c'est pas encore ça)
//===============================================
void moveCamera(Point* cameraPosition, Character* selectedCharacter)
{
  float movingRatio = 1;
  // Déplacement sur l'axe des x
  if (cameraPosition->x + selectedCharacter->position.x < -0.5)
    cameraPosition->x += 0.5*movingRatio;
  else if (cameraPosition->x + selectedCharacter->position.x > 0.5)
    cameraPosition->x -= 0.5*movingRatio;

  // Déplacement sur l'axe des y (pas dans le cas où le personnage saute)
  if (selectedCharacter->ySpeed == 0)
  {
    if (cameraPosition->y < -selectedCharacter->position.y - 0.5)
      cameraPosition->y += 0.5;
    else if (cameraPosition->y > -selectedCharacter->position.y - 0.5)
      cameraPosition->y -= 0.5;
  }
}
