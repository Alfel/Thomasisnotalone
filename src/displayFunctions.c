//===============================================
// displayFunctions.c
//-----------------------------------------------
// Fonctions d'affichage des composantes de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "displayFunctions.h"


//===============================================
// Dessiner un carré canonique plein (1) ou vide (0)
//===============================================
void displaySquare(int filled)
{
  glBegin(filled == 1 ? GL_QUADS : GL_LINE_LOOP);
  	glVertex2f(-0.5, 0.5);
  	glVertex2f(0.5, 0.5);
  	glVertex2f(0.5, -0.5);
  	glVertex2f(-0.5, -0.5);
  glEnd();
}


//===============================================
// Dessiner un triangle aux positions x et y données
//===============================================
void displayTriangle(float xPosition, float yPosition)
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


//===============================================
// Afficher un personnage
//===============================================
void displayCharacter(Character* character)
{
  glPushMatrix();
  	glColor3ub(character->color.red, character->color.green, character->color.blue);
  	glTranslatef(character->position.x, character->position.y, 0.);
  	glScalef(character->width, character->height, 1.);
  	displaySquare(1);
  glPopMatrix();
}


//===============================================
// Afficher la position finale d'un personnage
//===============================================
void displayFinalPosition(Character* character)
{
  glPushMatrix();
  	glColor3ub(255, 255, 255);
    glTranslatef(character->finalPosition.x, character->finalPosition.y, 0.);
  	glScalef(character->width, character->height, 1.);
  	displaySquare(0);
  glPopMatrix();
}


//===============================================
// Afficher l'avatar d'un personnage (en bas à droite)
//===============================================
void displayAvatar(Color color, int number, float ratio)
{
  glPushMatrix();
    glColor3ub(color.red, color.green, color.blue);
    glTranslatef(18*ratio-number*2, -18., 0.);
    glScalef(2., 2., 1.);
    displaySquare(1);
  glPopMatrix();
}


//===============================================
// Afficher un obstacle
//===============================================
void displayObstacle(Obstacle* obstacle)
{
	glPushMatrix();
	  glTranslatef(obstacle->position.x, obstacle->position.y, 0.);
	  glScalef(obstacle->width, obstacle->height, 1.);
	  displaySquare(1);
	glPopMatrix();
}


//===============================================
// Afficher une image sur tout l'écran de jeu (1) ou non (0)
//===============================================
void displayImage(GLuint imageName, int full, float windowInfo[])
{
  // Activation & sélection texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, imageName);

  float xPosition = UNIT, xRepetition = 1, yRepetition = 1;

  if (full) // Image sur tout l'écran de jeu = texture qui se répète
  {
    xPosition *= windowInfo[2];

    // Récupération de la largeur de la texture
    GLint patternWidth;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &patternWidth);
    
    // Calcul du nombre de répétitions
    xRepetition = windowInfo[0]/patternWidth;
    yRepetition = windowInfo[1]/patternWidth;
  }

  // Dessin
  glBegin(GL_QUADS);
    glTexCoord2f(0, yRepetition);
    glVertex2f(-xPosition, -UNIT);

    glTexCoord2f(xRepetition, yRepetition);
    glVertex2f(xPosition, -UNIT);

    glTexCoord2f(xRepetition, 0);
    glVertex2f(xPosition, UNIT);

    glTexCoord2f(0, 0);
    glVertex2f(-xPosition, UNIT);
  glEnd();

  // Désélection & désactivation texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}
