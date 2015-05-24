#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "gameFunctions.h"


// Dessiner un joli petit carré canonique
void drawSquare(int filled)
{
  glBegin(filled == 1 ? GL_QUADS : GL_LINE_LOOP);
  	glVertex2f(-0.5, 0.5);
  	glVertex2f(0.5, 0.5);
  	glVertex2f(0.5, -0.5);
  	glVertex2f(-0.5, -0.5);
  glEnd();
}


// Créer un nouveau personnage
Character* createCharacter(float width, float height, Color color, State state, float maxSpeed, Point position, Point finalPosition)
{
  Character* newCharacter = (Character*)calloc(1, sizeof(Character));

  newCharacter->width = width;
  newCharacter->height = height;
  newCharacter->color = color;
  newCharacter->state = state;

  newCharacter->xMoving = 0;
  newCharacter->yMoving = 0;
  newCharacter->maxSpeed = maxSpeed;

  newCharacter->position.x = position.x;
  newCharacter->position.y = position.y;

  newCharacter->finalPosition.x = finalPosition.x;
  newCharacter->finalPosition.y = finalPosition.y;
  
  return newCharacter;
}


// Afficher un personnage
void drawCharacter(Character* character)
{
  glPushMatrix();
  	glColor3ub(character->color.red, character->color.green, character->color.blue);
  	glTranslatef(character->position.x, character->position.y, 0.);
  	glScalef(character->width, character->height, 1.);
  	drawSquare(1);
  glPopMatrix();
}


// Afficher l'avatar d'un personnage (en bas à droite)
void drawAvatar(Color color, int number, float ratio)
{
  glPushMatrix();
    glColor3ub(color.red, color.green, color.blue);
    glTranslatef(18*ratio-number*2, -18., 0.);
    glScalef(2., 2., 1.);
    drawSquare(1);
  glPopMatrix();
}


// Afficher la position finale d'un personnage
void drawFinalPosition(Character* character)
{
  glPushMatrix();
  	glColor3ub(255, 255, 255);
    glTranslatef(character->finalPosition.x, character->finalPosition.y, 0.);
  	glScalef(character->width, character->height, 1.);
  	drawSquare(0);
  glPopMatrix();
}


// Faire sauter un personnage
/*void jump(Character* character)
{
  character->state = ALOFT;
}


void gravity(Character* character)
{
  if (character->state == ALOFT)
  {
  }
  character->yMoving += ;
}*/


// Déplacer un personnage
void moveCharacter(Character* character, int characterMoving, Direction motion, float* xMoving, Obstacle* obstaclesMap[], int size)
{
  const float SPACING = 0.1;
  const float ALOFT_SPACING = 0.05;

  // Si la touche du clavier est enfoncée (déplacement)
  if (characterMoving)
  {
    if (motion == LEFT)
      *xMoving = *xMoving < (SPACING - character->maxSpeed) ? character->maxSpeed * -1 : *xMoving - SPACING;  
    else if (motion == RIGHT)
      *xMoving = *xMoving > (character->maxSpeed - SPACING) ? character->maxSpeed : *xMoving + SPACING;
  }
  
  // Si la touche du clavier n'est plus enfoncée (décélération)
  else if (*xMoving != 0)
  {
    if (motion == LEFT)
      *xMoving = *xMoving > -SPACING ? 0 : *xMoving + 1.5*SPACING;
    else if (motion == RIGHT)
      *xMoving = *xMoving < SPACING ? 0 : *xMoving - 1.5*SPACING;
  }

  // Tests pour les collisions
  int i;
  for (i = 0; i < size; i++)
  {
    float ThomasLeft = character->position.x - character->width/2;
    float ThomasRight = character->position.x + character->width/2;
    float ThomasTop = character->position.y + character->height/2;
    float ThomasBottom = character->position.y - character->height/2;

    float ObstacleLeft = obstaclesMap[i]->position.x - obstaclesMap[i]->width/2;
    float ObstacleRight = obstaclesMap[i]->position.x + obstaclesMap[i]->width/2;
    float ObstacleTop = obstaclesMap[i]->position.y + obstaclesMap[i]->height/2;
    float ObstacleBottom = obstaclesMap[i]->position.y - obstaclesMap[i]->height/2;

    if (ThomasLeft - *xMoving < ObstacleRight && 
        ThomasRight + *xMoving > ObstacleLeft && 
        ThomasTop > ObstacleBottom && 
        ThomasBottom < ObstacleTop)
    {
      if (motion == LEFT)
      {
        character->position.x = ObstacleRight + character->width/2;
        *xMoving = 0;
      }

      else if (motion == RIGHT)
      {
        character->position.x = ObstacleLeft - character->width/2;
        *xMoving = 0;
      }
    }
  }

  // Positionnement du personnage
  character->position.x += *xMoving;
  //printf("%f\n", character->position.x);
}


// Sélectionner un personnage
void selectACharacter(Character* availableCharacters[], int* whichCharacter, Character** selectedCharacter, int nbCharacters)
{
  // Si tous les personnages sont arrivés à destination
  int stillCharacter = 0, i;
  for (i = 0; i < nbCharacters; ++i)
  {
    if (!isWhite(availableCharacters[i]))
      stillCharacter++;
  }
  if (!stillCharacter)
  {
    //printf("T'as gagné mon poulet\n");
    return;
  }

  // Vérification de la sélectionnabilité (oui j'invente des mots) du personnage
  while (isWhite(availableCharacters[*whichCharacter]))
    *whichCharacter = *whichCharacter == nbCharacters - 1 ? 0 : *whichCharacter + 1;

  // Sélection du personnage
	*selectedCharacter = availableCharacters[*whichCharacter];

  // Dessin du triangle au-dessus du personnage sélectionné
  float xPosition = (*selectedCharacter)->position.x;
  float yPosition = (*selectedCharacter)->position.y + (*selectedCharacter)->height / 2 + 0.5;
  drawTriangle(xPosition, yPosition);
}


// Dessiner un triangle au-dessus de l'avatar du personnage sélectionné
void selectAvatar(int whichCharacter, float ratio)
{
  drawTriangle(18*ratio - whichCharacter*2, -16.5);
}


// Indique si un personnage est blanc (retourne 1) ou non (retourne 0)
int isWhite(Character* character)
{
  if (character->color.red == 255 && 
    character->color.green == 255 && 
    character->color.blue == 255)
    return 1;
  return 0;
}


// Dessine un triangle aux positions x et y données
void drawTriangle(float xPosition, float yPosition)
{
  glPushMatrix();
    glColor3ub(255, 255, 255);
    glTranslatef(xPosition, yPosition, 1.);
    glBegin(GL_TRIANGLES);
      glVertex2f(-0.5, 0.5);
      glVertex2f(0.5, 0.5);
      glVertex2f(0, 0.);
    glEnd();
  glPopMatrix();
}


// Atteindre la position finale d'un personnage
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters)
{
  float characterRight = selectedCharacter->position.x + selectedCharacter->width/2;
  float characterLeft = selectedCharacter->position.x - selectedCharacter->width/2;

  if (characterRight > selectedCharacter->finalPosition.x &&
    characterLeft < selectedCharacter->finalPosition.x)
  {
    // Modification de la couleur du personnage
    selectedCharacter->color = setColor(255, 255, 255);

    // Sélection du personnage suivant
    *whichCharacter = *whichCharacter == nbCharacters - 1 ? 0 : *whichCharacter + 1;
  }
}


// Bouger la caméra de manière douce et agréable, en toute sérénité
// (Bon c'est pas encore ça)
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


float getMovingRatio(Point cameraPosition, float xToReach, float yToReach)
{
  float xDistance = cameraPosition.x - xToReach;
  float yDistance = cameraPosition.y - yToReach;

  while (!fmod(xDistance, 10) && !fmod(yDistance, 10))
  {
    xDistance /= 10;
    yDistance /= 10;
  }

  float movingRatio = fabs(xDistance / yDistance);
  //printf("%f\n", movingRatio);
  return movingRatio;
}


void startGame(int level, float ratio)
{
  Game game;
  SDL_Event event;
  int loop = 1, characterMoving = 0;
  Direction motion;
  float xMoving = 0;

  char filePath[15] = {0};  
  sprintf(filePath, "lvl/level%d.lvl", level);

  printf("%s\n", filePath);

  setGame(&game, filePath);
  SDL_EnableKeyRepeat(100, 100);

  while (loop)
  {
    glClear(GL_COLOR_BUFFER_BIT);

    loadGame(&game, ratio, &xMoving, characterMoving, motion);

    SDL_GL_SwapBuffers();
        
    while (SDL_PollEvent(&event))
    {
      switch(event.type)
      {
        case SDL_QUIT:
          loop = 0;
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