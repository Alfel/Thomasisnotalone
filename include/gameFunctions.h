//===============================================
// gameFunctions.h
//-----------------------------------------------
// Prototypes des fonctions d'initialisation, démarrage et affichage du jeu lui-même
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __GAME_FUNCTIONS__H
#define __GAME_FUNCTIONS__H

#include "settingFunctions.h"
#include "playingFunctions.h"


typedef struct Game
{
	int nbCharacters, nbObstacles, whichCharacter;
	Character** availableCharacters; // équivalent à Character* availableCharacters[x]
	Character* selectedCharacter;
	Obstacle** obstaclesMap;
	Color* charactersColor;
	Color obstaclesColor;
	Point cameraPosition;
} Game;


void loadGame(Game* game, float ratio, float* xMoving, int characterMoving, Direction motion);
void startGame(int level, float ratio);
void setGame(Game* newGame, const char filePath[]);
void moveCamera(Point* cameraPosition, Character* selectedCharacter, float movingRatio);


#endif
