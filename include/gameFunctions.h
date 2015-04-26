#ifndef __GAMEFUNCTIONS__H
#define __GAMEFUNCTIONS__H

#include "sceneFunctions.h"

typedef struct Character
{
	float width;
	float height;
	Color color;
	float maxSpeed;
	Point position;
	Point finalPosition;
} Character;

typedef enum Direction Direction;
enum Direction { LEFT, RIGHT };

void drawSquare(int filled);
Character* createCharacter(float width, float height, Color color, float maxSpeed, Point position, Point finalPosition);
void drawCharacter(Character* character);
void drawFinalPosition(Character* character);
void moveCharacter(Character* character, int characterMoving, Direction motion, float* xMoving, Obstacle* obstaclesMap[], int size);
void selectACharacter(Character* availableCharacters[], int* whichCharacter, Character** selectedCharacter, int nbCharacters);
void drawTriangle(float xPosition, float yPosition);
void reachFinalPosition(Character* selectedCharacter, Character* availableCharacters[], int* whichCharacter, int nbCharacters);
int isWhite(Character* character);
void drawAvatar(Color color, int number, float ratio);
void selectAvatar(int whichCharacter, float ratio);
void moveCamera(Point* cameraPosition, Character* selectedCharacter, float ratio);

#endif
