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
void loadGame(Game* game, float ratio, float* xMoving, int characterMoving, Direction motion)
{
  int i;
  float movingRatio = 1;


  glPushMatrix();
    
    moveCamera(&game->cameraPosition, game->selectedCharacter, movingRatio);
    glTranslatef(game->cameraPosition.x, game->cameraPosition.y, 0.);
    moveCharacter(game->selectedCharacter, characterMoving, motion, xMoving, game->obstaclesMap, game->nbObstacles);
    selectACharacter(game->availableCharacters, &game->whichCharacter, &game->selectedCharacter, game->nbCharacters);
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
void startGame(int level, float windowInfo[])
{
  Game game;
  SDL_Event event;
  int loop = 1, characterMoving = 0;
  Direction motion;
  float xMoving = 0;

  setGame(&game, level);
  GLuint pattern = loadImage(game.pattern);

  SDL_EnableKeyRepeat(100, 100);

  while (loop)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    displayImage(pattern, 1, windowInfo);
    loadGame(&game, windowInfo[2], &xMoving, characterMoving, motion);

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

            //case SDLK_UP:
            //case 'z':
            //int characterYMoving = 1;

            case SDLK_TAB:
            characterMoving = 0;
            game.whichCharacter = game.whichCharacter == game.nbCharacters - 1 ? 0 : game.whichCharacter + 1;
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
          //characterYMoving = 0;

        default:
          break;
      }
    }
  }
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
    float width, height, maxSpeed, x, y, finalX, finalY;
    int r, g, b, state;
    fscanf(file, "%f %f %d %d %d %d %f %f %f %f %f", &width, &height, &r, &g, &b, &state, &maxSpeed, &x, &y, &finalX, &finalY);
    newGame->availableCharacters[i] = setCharacter(width, height, setColor(r, g, b), (State)state, maxSpeed, setPosition(x, y), setPosition(finalX, finalY));
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
void moveCamera(Point* cameraPosition, Character* selectedCharacter, float movingRatio)
{
  // Déplacement sur l'axe des x
  if (cameraPosition->x + selectedCharacter->position.x < -0.5)
    cameraPosition->x += 0.5*movingRatio;
  else if (cameraPosition->x + selectedCharacter->position.x > 0.5)
    cameraPosition->x -= 0.5*movingRatio;

  // Déplacement sur l'axe des y
  if (cameraPosition->y < -selectedCharacter->position.y - 0.5)
    cameraPosition->y += 0.5;
  else if (cameraPosition->y > -selectedCharacter->position.y - 0.5)
    cameraPosition->y -= 0.5;
}
