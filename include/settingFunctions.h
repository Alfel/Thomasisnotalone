//===============================================
// settingFunctions.h
//-----------------------------------------------
// Prototypes des fonctions d'initialisation des composantes de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#ifndef __SETTING_FUNCTIONS__H
#define __SETTING_FUNCTIONS__H


typedef enum State { GROUND, ALOFT } State;

typedef enum Direction Direction;
enum Direction { LEFT, RIGHT };


typedef struct Point
{
	float x;
	float y;
} Point;


typedef struct Color
{
	int red;
	int green;
	int blue;
} Color;


typedef struct Character
{
	float width, height, xMoving, yMoving;
	Color color;
	State state;
	float maxSpeed;
	Point position, finalPosition;
} Character;


typedef struct Obstacle
{
	float width;
	float height;
	Point position;
} Obstacle;


Point setPosition(float x, float y);
Color setColor(int red, int green, int blue);
Character* setCharacter(float width, float height, Color color, State state, float maxSpeed, Point position, Point finalPosition);
Obstacle* setObstacle(float width, float height, Point position);
GLuint loadImage(const char imagePath[]);


#endif
