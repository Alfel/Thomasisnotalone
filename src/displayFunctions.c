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
// Dessiner un carré canonique plein ou vide
//===============================================
void displaySquare(Bool filled)
{
  glBegin((filled == TRUE) ? GL_QUADS : GL_LINE_LOOP);
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
    glColor3ub(202, 210, 221);
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
  	displaySquare(TRUE);
  glPopMatrix();
}


//===============================================
// Afficher la position finale d'un personnage
//===============================================
void displayFinalPosition(Character* character)
{
  glPushMatrix();
  	glColor3ub(202, 210, 221);
    glTranslatef(character->finalPosition.x, character->finalPosition.y, 0.);
  	glScalef(character->width, character->height, 1.);
  	displaySquare(FALSE);
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
    displaySquare(TRUE);
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
	  displaySquare(TRUE);
	glPopMatrix();
}


//===============================================
// Afficher une image sur tout l'écran de jeu ou non
//===============================================
void displayImage(GLuint imageName, Bool full, Window window)
{
  // Activation & sélection texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, imageName);

  float xPosition = UNIT, xRepetition = 1, yRepetition = 1;

  if (full == TRUE) // Image sur tout l'écran de jeu = texture qui se répète
  {
    xPosition *= window.ratio;

    // Récupération de la largeur de la texture
    GLint patternWidth;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &patternWidth);
    
    // Calcul du nombre de répétitions
    xRepetition = (window.fullScreen == FALSE) ? window.width/patternWidth : window.screenWidth/patternWidth;
    yRepetition = (window.fullScreen == FALSE) ? window.height/patternWidth : window.screenHeight/patternWidth;
  }

  // Dessin
  glColor3ub(255, 255, 255);
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
