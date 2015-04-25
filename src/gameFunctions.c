#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
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
Character* createCharacter(float width, float height, Color color, float maxSpeed, Point position, Point finalPosition)
{
  Character* newCharacter = (Character*)calloc(1, sizeof(Character));

  newCharacter->width = width;
  newCharacter->height = height;

  newCharacter->color = color;

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
void drawAvatar(Color color)
{
  glPushMatrix();
    glColor3ub(color.red, color.green, color.blue);
    glTranslatef(20, -19, 0.);
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


// Déplacer un personnage
void moveCharacter(Character* character, int characterMoving, Direction motion, float* xMoving, Obstacle* obstaclesMap[], int size)
{
  const float SPACING = 0.1;

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

    if (motion == LEFT)
    {
      if (ThomasLeft - *xMoving < ObstacleRight && 
        ThomasRight + *xMoving > ObstacleLeft && 
        ThomasTop > ObstacleBottom && 
        ThomasBottom < ObstacleTop)
      {
        character->position.x = ObstacleRight + character->width/2;
        *xMoving = 0;
        //*xMoving *= -1;
      }
    }
  }

  // Positionnement du personnage
  character->position.x += *xMoving;
}


// Sélectionner un personnage
void selectACharacter(Character* availableCharacters[], int* whichCharacter, Character** selectedCharacter)
{
	*selectedCharacter = availableCharacters[*whichCharacter];

  float xPosition = (*selectedCharacter)->position.x;
  float yPosition = (*selectedCharacter)->position.y + (*selectedCharacter)->height / 2 + 0.5;
  drawTriangle(xPosition, yPosition);

  drawTriangle();
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
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter)
{
  float characterRight = selectedCharacter->position.x + selectedCharacter->width/2;
  float characterLeft = selectedCharacter->position.x - selectedCharacter->width/2;

  if (characterRight > selectedCharacter->finalPosition.x &&
    characterLeft < selectedCharacter->finalPosition.x)
  {
    // Modification de la couleur du personnage
    selectedCharacter->color = setColor(255, 255, 255);

    // Suppression du personnage dans le tableau
    availableCharacters[*whichCharacter] = NULL;

    *whichCharacter += 1;
  }


  /*while (availableCharacters[*whichCharacter] == NULL)
  {
    *whichCharacter += 1;
    *whichCharacter = *whichCharacter == 1 ? 0 : *whichCharacter + 1;
  }*/
}