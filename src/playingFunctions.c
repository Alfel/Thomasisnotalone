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
#include <math.h>

#include "playingFunctions.h"


//===============================================
// Sélectionner un personnage
//===============================================
void selectACharacter(Character* availableCharacters[], int whichCharacter, Character** selectedCharacter, int nbCharacters, Bool* loop)
{
  // Si tous les personnages sont arrivés à destination
  Bool stillCharacter = FALSE;
  int i;
  for (i = 0; i < nbCharacters; ++i)
  {
    if (isWhite(availableCharacters[i]) == FALSE)
      stillCharacter = TRUE;
  }
  if (stillCharacter == FALSE)
  {
    *loop = TRUE;
    return;
  }

  // Sélection du personnage
	*selectedCharacter = availableCharacters[whichCharacter];

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
// Déplacer le personnage sélectionné vers la gauche
//===============================================
void moveLeft(Character* character, Bool xMoving)
{
  if (xMoving == TRUE)
    character->xSpeed = -0.5;
    //character->xSpeed = (character->xSpeed < (SPACING - MAX_SPEED)) ? MAX_SPEED * -1 : character->xSpeed - SPACING;
}


//===============================================
// Déplacer le personnage sélectionné vers la droite
//===============================================
void moveRight(Character* character, Bool xMoving)
{
  if (xMoving == TRUE)
    character->xSpeed = 0.5;
    //character->xSpeed = (character->xSpeed > (MAX_SPEED - SPACING)) ? MAX_SPEED : character->xSpeed + SPACING;
}

//===============================================
// Appliquer la physique du jeu à tous les personnages
//===============================================
void moveCharacters(Character* characters[], Obstacle* obstaclesMap[], int nbCharacters, int nbObstacles, int whichCharacter, Bool* yMoving)
{
  float gravityForce = 0.03;

  int i, j, k;

  for (i = 0; i < nbCharacters; ++i)
  {
    // Gravité
    characters[i]->ySpeed = characters[i]->ySpeed < (gravityForce - MAX_SPEED) ? MAX_SPEED * -1 : characters[i]->ySpeed - gravityForce;

    // Positionnement du personnage
    characters[i]->position.x += characters[i]->xSpeed;
    characters[i]->position.y += characters[i]->ySpeed;

    // Collisions avec les obstacles
    for (j = 0; j < nbObstacles; ++j)
    {
      float w = 0.5 * (obstaclesMap[j]->width + characters[i]->width);
      float h = 0.5 * (obstaclesMap[j]->height + characters[i]->height);
      float dx = obstaclesMap[j]->position.x - characters[i]->position.x;
      float dy = obstaclesMap[j]->position.y - characters[i]->position.y;

      if (fabs(dx) <= w && fabs(dy) <= h)
      {
        float wy = w * dy;
        float hx = h * dx;

        if (wy > hx)
        {
          if (wy > -hx)
          {
            // Collision par le haut
            float obstacleBottom = obstaclesMap[j]->position.y - obstaclesMap[j]->height/2;
            characters[i]->position.y = obstacleBottom - characters[i]->height/2;
            characters[i]->ySpeed = 0.;
          }
          else
          {
            // Collision par la gauche
            float obstacleRight = obstaclesMap[j]->position.x + obstaclesMap[j]->width/2;
            characters[i]->position.x = obstacleRight + characters[i]->width/2;
          }
        }
        else      
        {
          if (wy > -hx)
          {
            // Collision par la droite
            float obstacleLeft = obstaclesMap[j]->position.x - obstaclesMap[j]->width/2;
            characters[i]->position.x = obstacleLeft - characters[i]->width/2;
          }
          else
          {
            // Collision par le bas
            float obstacleTop = obstaclesMap[j]->position.y + obstaclesMap[j]->height/2;
            characters[i]->position.y = obstacleTop + characters[i]->height/2;
            characters[i]->ySpeed = 0.;
            *yMoving = FALSE;
            characters[i]->state = GROUND;
          }
        }
      }
    }

    // Collisions avec les autres personnages
    for (k = 0; k < nbCharacters; ++k)
    {
      float w = 0.5 * (characters[k]->width + characters[i]->width);
      float h = 0.5 * (characters[k]->height + characters[i]->height);
      float dx = characters[k]->position.x - characters[i]->position.x;
      float dy = characters[k]->position.y - characters[i]->position.y;

      if (k != i && fabs(dx) <= w && fabs(dy) <= h)
      {
        float wy = w * dy;
        float hx = h * dx;

        if (wy > hx)
        {
          if (wy > -hx)
          {
            // Collision par le haut
            float obstacleBottom = characters[k]->position.y - characters[k]->height/2;
            characters[i]->position.y = obstacleBottom - characters[i]->height/2;
            characters[i]->ySpeed = 0.;
          }
          else
          {
            // Collision par la gauche
            float obstacleRight = characters[k]->position.x + characters[k]->width/2;
            characters[i]->position.x = obstacleRight + characters[i]->width/2;
          }
        }
                
        else
        {
          if (wy > -hx)
          {
            // Collision par la droite
            float obstacleLeft = characters[k]->position.x - characters[k]->width/2;
            characters[i]->position.x = obstacleLeft - characters[i]->width/2;
          }
            
          else
          {
            // Collision par le bas
            float obstacleTop = characters[k]->position.y + characters[k]->height/2;
            characters[i]->position.y = obstacleTop + characters[i]->height/2;
            characters[i]->ySpeed = 0.;
            characters[i]->state = GROUND;
            if (i != whichCharacter)
              characters[i]->xSpeed = characters[k]->xSpeed;
          }
        }
      }
    }
  }
}


//===============================================
// Atteindre la position finale d'un personnage
//===============================================
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters)
{
  float characterRight = selectedCharacter->position.x + selectedCharacter->width/2;
  float characterLeft = selectedCharacter->position.x - selectedCharacter->width/2;
  float characterTop = selectedCharacter->position.y + selectedCharacter->height/2;
  float characterBottom = selectedCharacter->position.y - selectedCharacter->height/2;

  if (characterRight > selectedCharacter->finalPosition.x &&
    characterLeft < selectedCharacter->finalPosition.x &&
    characterTop > selectedCharacter->finalPosition.y &&
    characterBottom < selectedCharacter->finalPosition.y &&
    isWhite(selectedCharacter) == FALSE)
  {
    // Modification de la couleur du personnage
    selectedCharacter->color = setColor(202, 210, 221);

    // Mise à zéro de sa vitesse
    selectedCharacter->xSpeed = 0;
    selectedCharacter->ySpeed = 0;

    // Sélection du personnage suivant
    *whichCharacter = *whichCharacter == nbCharacters - 1 ? 0 : *whichCharacter + 1;
  }
}


//===============================================
// Savoir si un personnage est blanc (1) ou non (0)
//===============================================
Bool isWhite(Character* character)
{
  if (character->color.red == 202 && 
    character->color.green == 210 && 
    character->color.blue == 221)
    return TRUE;
  return FALSE;
}


// Faire sauter un personnage
void jump(Character* character, Bool yMoving)
{
  if (yMoving == TRUE && character->state == GROUND)
  {
    character->ySpeed = character->jumpPower;
    character->state = ALOFT;
  }
}
