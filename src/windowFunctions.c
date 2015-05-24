//===============================================
// windowFunctions.c
//-----------------------------------------------
// Fonctions de rendu de la fenêtre
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "windowFunctions.h"


//===============================================
// Redimmensionner la fenêtre
//===============================================
void reshape(float windowWidth, float windowHeight, float* ratio)
{
	*ratio = windowWidth / windowHeight;

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-UNIT*(*ratio), UNIT*(*ratio), -UNIT, UNIT);
}


//===============================================
// Initialiser la fenêtre
//===============================================
void setVideoMode(float windowWidth, float windowHeight, float* ratio)
{
	if (!SDL_SetVideoMode(windowWidth, windowHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE))
	{
		fprintf(stderr, "Can't open window. End of program.\n");
		exit(EXIT_FAILURE);
	}
  reshape(windowWidth, windowHeight, ratio);
}
