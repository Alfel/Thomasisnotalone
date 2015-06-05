//===============================================
// windowFunctions.h
//-----------------------------------------------
// Prototypes des fonctions de rendu de la fenêtre
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __WINDOW_FUNCTIONS__H
#define __WINDOW_FUNCTIONS__H


#define BIT_PER_PIXEL 32
#define FRAMERATE_MILLISECONDS 1000 / 60
#define UNIT 20


typedef enum Bool { FALSE, TRUE } Bool;


typedef struct Window
{
	float screenWidth, screenHeight, width, height, ratio;
	Bool loop, fullScreen;
} Window;


Window setWindow();
void reshape(float windowWidth, float windowHeight, float* ratio);
void setVideoMode(Window* window);


#endif
