#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "sceneFunctions.h"
#include "gameFunctions.h"



void reshape(float windowWidth, float windowHeight, float* ratio)
{
	*ratio = windowWidth / windowHeight;

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-20*(*ratio), 20*(*ratio), -20., 20.);
}


void setVideoMode(float windowWidth, float windowHeight, float* ratio)
{
	if (NULL == SDL_SetVideoMode(windowWidth, windowHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE))
	{
		fprintf(stderr, "Impossible d'ouvrir la fenetre. Fin du programme.\n");
		exit(EXIT_FAILURE);
	}
  reshape(windowWidth, windowHeight, ratio);
}


// Initialiser un point à partir de ses coordonnées x et y
Point setPosition(float x, float y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}


// Initialiser une couleur à partir de ses composantes rvb
Color setColor(int red, int green, int blue)
{
	Color c;
	c.red = red;
	c.green = green;
	c.blue = blue;
	return c;
}


// Créer un nouvel obstacle
Obstacle* createObstacle(float width, float height, Point position)
{
  Obstacle* newObstacle = (Obstacle*)calloc(1, sizeof(Obstacle));

  newObstacle->width = width;
  newObstacle->height = height;

  newObstacle->position.x = position.x;
  newObstacle->position.y = position.y;
  
  return newObstacle;
}


// Afficher les obstacles
void drawObstacle(Obstacle* obstacle)
{
	glPushMatrix();
	  glColor3ub(255, 255, 255);
	  glTranslatef(obstacle->position.x, obstacle->position.y, 0.);
	  glScalef(obstacle->width, obstacle->height, 1.);
	  drawSquare(1);
	glPopMatrix();
}
