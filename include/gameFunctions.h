//===============================================
// gameFunctions.h
//-----------------------------------------------
// Prototypes des fonctions d'initialisation, démarrage et affichage du jeu lui-même
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __GAME_FUNCTIONS__H
#define __GAME_FUNCTIONS__H

#include "windowFunctions.h"
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
	char pattern[17];
	Point cameraPosition;
} Game;


void loadGame(Game* game, float ratio, Bool xMoving, Bool* yMoving, Bool* loop);
void startGame(int level, Window* window);
void setGame(Game* newGame, int level);
void moveCamera(Point* cameraPosition, Character* selectedCharacter);


#endif
