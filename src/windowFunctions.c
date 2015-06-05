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
// Initialisation des informations nécessaire au bon affichage de la fenêtre
//===============================================
Window setWindow()
{
	Window w;

	// Récupération des dimensions de l'écran (pour le mode plein écran)
	w.screenWidth = SDL_GetVideoInfo()->current_w;
  w.screenHeight = SDL_GetVideoInfo()->current_h;
  w.fullScreen = FALSE;

  w.width = 800;
  w.height = 600;
  w.ratio = w.width / w.height;

  // Pour pouvoir quitter la boucle du main depuis le jeu
  w.loop = TRUE;

	return w;
}


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
void setVideoMode(Window* window)
{
	SDL_Surface* gameSurface = SDL_SetVideoMode(window->width, window->height, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE);

	if (window->fullScreen == TRUE)
		gameSurface = SDL_SetVideoMode(window->screenWidth, window->screenHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_FULLSCREEN);

	if (!gameSurface)
	{
		fprintf(stderr, "Can't open window. End of program.\n");
		exit(EXIT_FAILURE);
	}

	if (window->fullScreen == TRUE)
  	reshape(window->screenWidth, window->screenHeight, &window->ratio);
  else
  	reshape(window->width, window->height, &window->ratio); 
}
