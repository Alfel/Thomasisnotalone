//===============================================
// playingFunctions.h
//-----------------------------------------------
// Prototypes des fonctions de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __PLAYING_FUNCTIONS__H
#define __PLAYING_FUNCTIONS__H

#include "settingFunctions.h"
#include "displayFunctions.h"


void selectACharacter(Character* availableCharacters[], int* whichCharacter, Character** selectedCharacter, int nbCharacters);
void selectAvatar(int whichCharacter, float ratio);
void moveCharacter(Character* character, int characterMoving, Direction motion, float* xMoving, Obstacle* obstaclesMap[], int size);
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters);
int isWhite(Character* character);


#endif
