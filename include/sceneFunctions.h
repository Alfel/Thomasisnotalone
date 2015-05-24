#ifndef __SCENEFUNCTIONS__H
#define __SCENEFUNCTIONS__H

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

typedef struct Obstacle
{
	float width;
	float height;
	Point position;
} Obstacle;

static const unsigned int BIT_PER_PIXEL = 32;
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

void reshape(float windowWidth, float windowHeight, float* ratio);
void setVideoMode(float windowWidth, float windowHeight, float* ratio);

Point setPosition(float x, float y);
Color setColor(int red, int green, int blue);

Obstacle* createObstacle(float width, float height, Point position);
void drawObstacle(Obstacle* obstacle);

GLuint loadImage(const char imagePath[]);
void showImage(GLuint imageName, int full, float ratio);

#endif
