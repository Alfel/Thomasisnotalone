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


#define MAX_SPEED 2
#define SPACING 0.2


void selectACharacter(Character* availableCharacters[], int whichCharacter, Character** selectedCharacter, int nbCharacters, Bool* loop);
void selectAvatar(int whichCharacter, float ratio);
void moveCharacters(Character* availableCharacters[], Obstacle* obstaclesMap[], int nbCharacters, int nbObstacles, int whichCharacter, Bool* yMoving);
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters);
Bool isWhite(Character* character);
void jump(Character* character, Bool yMoving);
void moveLeft(Character* character, Bool xMoving);
void moveRight(Character* character, Bool xMoving);


#endif
