//===============================================
// playingFunctions.c
//-----------------------------------------------
// Fonctions de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "playingFunctions.h"


//===============================================
// Sélectionner un personnage
//===============================================
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
  displayTriangle(xPosition, yPosition);
}


//===============================================
// Dessiner un triangle au-dessus de l'avatar du personnage sélectionné
//===============================================
void selectAvatar(int whichCharacter, float ratio)
{
  displayTriangle(18*ratio - whichCharacter*2, -16.5);
}


//===============================================
// Déplacer un personnage
//===============================================
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


//===============================================
// Atteindre la position finale d'un personnage
//===============================================
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


//===============================================
// Savoir si un personnage est blanc (1) ou non (0)
//===============================================
int isWhite(Character* character)
{
  if (character->color.red == 255 && 
    character->color.green == 255 && 
    character->color.blue == 255)
    return 1;
  return 0;
}


/*float getMovingRatio(Point cameraPosition, float xToReach, float yToReach)
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
}*/




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