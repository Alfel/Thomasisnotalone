#ifndef __GAMEFUNCTIONS__H
#define __GAMEFUNCTIONS__H

#include "sceneFunctions.h"

typedef enum State State;
enum State { GROUND, ALOFT };

typedef struct Character
{
	float width, height, xMoving, yMoving;
	Color color;
	State state;
	float maxSpeed;
	Point position, finalPosition;
} Character;

typedef enum Direction Direction;
enum Direction { LEFT, RIGHT };

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

void drawSquare(int filled);
Character* createCharacter(float width, float height, Color color, State state, float maxSpeed, Point position, Point finalPosition);
void drawCharacter(Character* character);
void drawFinalPosition(Character* character);
void moveCharacter(Character* character, int characterMoving, Direction motion, float* xMoving, Obstacle* obstaclesMap[], int size);
void selectACharacter(Character* availableCharacters[], int* whichCharacter, Character** selectedCharacter, int nbCharacters);
void drawTriangle(float xPosition, float yPosition);
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters);
int isWhite(Character* character);
void drawAvatar(Color color, int number, float ratio);
void selectAvatar(int whichCharacter, float ratio);
void moveCamera(Point* cameraPosition, Character* selectedCharacter, float movingRatio);

void loadGame(Game* game, float ratio, float* xMoving, int characterMoving, Direction motion);


#endif
