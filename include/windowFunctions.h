//===============================================
// windowFunctions.h
//-----------------------------------------------
// Prototypes des fonctions de rendu de la fenêtre
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __WINDOW_FUNCTIONS__H
#define __WINDOW_FUNCTIONS__H


static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
static const int UNIT = 20;


void reshape(float windowWidth, float windowHeight, float* ratio);
void setVideoMode(float windowWidth, float windowHeight, float* ratio);


#endif
