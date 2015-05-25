//===============================================
// displayFunctions.h
//-----------------------------------------------
// Prototypes des fonctions d'affichage des composantes de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __DISPLAY_FUNCTIONS__H
#define __DISPLAY_FUNCTIONS__H

#include "windowFunctions.h"
#include "settingFunctions.h"


void displaySquare(int filled);
void displayTriangle(float xPosition, float yPosition);
void displayCharacter(Character* character);
void displayFinalPosition(Character* character);
void displayAvatar(Color color, int number, float ratio);
void displayObstacle(Obstacle* obstacle);
void displayImage(GLuint imageName, int full, float windowInfo[]);


#endif
