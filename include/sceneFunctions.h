#ifndef __SCENEFUNCTIONS__H
#define __SCENEFUNCTIONS__H

typedef struct Point Point;
struct Point
{
	float x;
	float y;
};

typedef struct Color Color;
struct Color
{
	int red;
	int green;
	int blue;
};

typedef struct Obstacle Obstacle;
struct Obstacle
{
	float width;
	float height;
	Point position;
};

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(float windowWidth, float windowHeight);
void setVideoMode(float windowWidth, float windowHeight);

Point setPosition(float x, float y);
Color setColor(int red, int green, int blue);

Obstacle* createObstacle(float width, float height, Point position);
void drawObstacles(Obstacle* obstacles[], int size);

#endif
