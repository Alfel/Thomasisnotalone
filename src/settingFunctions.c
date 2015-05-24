//===============================================
// settingFunctions.c
//-----------------------------------------------
// Fonctions d'initialisation des composantes de jeu
// Alexander FELLER - Nancy RADJAYA - 2015
//===============================================


#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

#include "settingFunctions.h"


//===============================================
// Initialiser un point à partir de ses coordonnées x et y
//===============================================
Point setPosition(float x, float y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}


//===============================================
// Initialiser une couleur à partir de ses composantes rvb
//===============================================
Color setColor(int red, int green, int blue)
{
	Color c;
	c.red = red;
	c.green = green;
	c.blue = blue;
	return c;
}


//===============================================
// Initialiser un nouveau personnage
//===============================================
Character* setCharacter(float width, float height, Color color, State state, float maxSpeed, Point position, Point finalPosition)
{
  Character* newCharacter = (Character*)calloc(1, sizeof(Character));

  newCharacter->width = width;
  newCharacter->height = height;
  newCharacter->color = color;
  newCharacter->state = state;

  newCharacter->xMoving = 0;
  newCharacter->yMoving = 0;
  newCharacter->maxSpeed = maxSpeed;

  newCharacter->position.x = position.x;
  newCharacter->position.y = position.y;

  newCharacter->finalPosition.x = finalPosition.x;
  newCharacter->finalPosition.y = finalPosition.y;
  
  return newCharacter;
}


//===============================================
// Initialiser un nouvel obstacle
//===============================================
Obstacle* setObstacle(float width, float height, Point position)
{
  Obstacle* newObstacle = (Obstacle*)calloc(1, sizeof(Obstacle));

  newObstacle->width = width;
  newObstacle->height = height;

  newObstacle->position.x = position.x;
  newObstacle->position.y = position.y;
  
  return newObstacle;
}


//===============================================
// Charger une image/texture
//===============================================
GLuint loadImage(const char imagePath[])
{
  // Chargement de l'image
  SDL_Surface* image = IMG_Load(imagePath);
  if (!image)
  {
    fprintf(stderr, "Can't load image. End of program.\n");
    return EXIT_FAILURE;
  }

  GLuint imageName;
  glGenTextures(1, &imageName); // Génération d'un numéro de texture
  glBindTexture(GL_TEXTURE_2D, imageName);  // Sélection du numéro de texture

  // Récupération du format de l'image
  GLenum format;
  switch (image->format->BytesPerPixel)
  {
    case 1:
      format = GL_RED;
      break;
    case 3:
      format = GL_RGB;
      break;
    case 4:
      format = GL_RGBA;
      break;
    default:
      fprintf(stderr, "Can't support image format. End of program.\n");
      return EXIT_FAILURE;
  }

  // Transmission de l'image (type, mipmap, couleurs, largeur, hauteur, bordure, format, type des couleurs, adresse image)
  glTexImage2D(GL_TEXTURE_2D, 0, 4, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
    
  SDL_FreeSurface(image);
  
  return imageName;
}
