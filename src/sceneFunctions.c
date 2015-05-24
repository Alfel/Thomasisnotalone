#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include "sceneFunctions.h"
#include "gameFunctions.h"

#define UNIT 20.


void reshape(float windowWidth, float windowHeight, float* ratio)
{
	*ratio = windowWidth / windowHeight;

	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-UNIT*(*ratio), UNIT*(*ratio), -UNIT, UNIT);
}


void setVideoMode(float windowWidth, float windowHeight, float* ratio)
{
	if (!SDL_SetVideoMode(windowWidth, windowHeight, BIT_PER_PIXEL, SDL_OPENGL | SDL_RESIZABLE))
	{
		fprintf(stderr, "Can't open window. End of program.\n");
		exit(EXIT_FAILURE);
	}
  reshape(windowWidth, windowHeight, ratio);
}


// Initialiser un point à partir de ses coordonnées x et y
Point setPosition(float x, float y)
{
	Point p;
	p.x = x;
	p.y = y;
	return p;
}


// Initialiser une couleur à partir de ses composantes rvb
Color setColor(int red, int green, int blue)
{
	Color c;
	c.red = red;
	c.green = green;
	c.blue = blue;
	return c;
}


// Créer un nouvel obstacle
Obstacle* createObstacle(float width, float height, Point position)
{
  Obstacle* newObstacle = (Obstacle*)calloc(1, sizeof(Obstacle));

  newObstacle->width = width;
  newObstacle->height = height;

  newObstacle->position.x = position.x;
  newObstacle->position.y = position.y;
  
  return newObstacle;
}


// Afficher les obstacles
void drawObstacle(Obstacle* obstacle)
{
	glPushMatrix();
	  glTranslatef(obstacle->position.x, obstacle->position.y, 0.);
	  glScalef(obstacle->width, obstacle->height, 1.);
	  drawSquare(1);
	glPopMatrix();
}


// Initialiser la scène de jeu selon le fichier correspondant au niveau choisi
void setGame(Game* newGame, const char filePath[])
{
  int i;

	// Ouverture du fichier (r pour read only)
  FILE* file = fopen(filePath, "r");
  if (!file)
  	exit(EXIT_FAILURE);

  // Récupération du nombre de personnages & allocation des tableaux en dépendant
  fscanf(file, "%d", &newGame->nbCharacters);
  newGame->availableCharacters = calloc(newGame->nbCharacters, sizeof(Character*));
  if (!newGame->availableCharacters)
  	exit(EXIT_FAILURE);
  newGame->charactersColor = calloc(newGame->nbCharacters, sizeof(Color));
  if (!newGame->charactersColor)
  	exit(EXIT_FAILURE);

  // Initialisation des personnages & stockage de leur couleur
  for (i = 0; i < newGame->nbCharacters; ++i)
  {
  	float width, height, maxSpeed, x, y, finalX, finalY;
		int r, g, b, state;
  	fscanf(file, "%f %f %d %d %d %d %f %f %f %f %f", &width, &height, &r, &g, &b, &state, &maxSpeed, &x, &y, &finalX, &finalY);
  	newGame->availableCharacters[i] = createCharacter(width, height, setColor(r, g, b), (State)state, maxSpeed, setPosition(x, y), setPosition(finalX, finalY));
  	newGame->charactersColor[i] = setColor(r, g, b);
  }

  // Récupération du nombre d'obstacles (+ couleur) & allocation du tableau en dépendant
  fscanf(file, "%d", &newGame->nbObstacles);
	int r, g, b;
  fscanf(file, "%d %d %d", &r, &g, &b);
  newGame->obstaclesColor = setColor(r, g, b);
  newGame->obstaclesMap = calloc(newGame->nbObstacles, sizeof(Obstacle*));
  if (!newGame->obstaclesMap)
  	exit(EXIT_FAILURE);
  
  // Initialisation des obstacles
  for (i = 0; i < newGame->nbObstacles; ++i)
  {
  	float width, height, x, y;
  	fscanf(file, "%f %f %f %f", &width, &height, &x, &y);
  	newGame->obstaclesMap[i] = createObstacle(width, height, setPosition(x, y));
  }

  // Fermeture du fichier
  fclose(file);

  newGame->whichCharacter = 0;
  newGame->selectedCharacter = newGame->availableCharacters[newGame->whichCharacter];

  // Position caméra
  newGame->cameraPosition = setPosition(-newGame->selectedCharacter->position.x, -newGame->selectedCharacter->position.y);
}


// Charger et afficher la scène de jeu
void loadGame(Game* game, float ratio, float* xMoving, int characterMoving, Direction motion)
{
	int i;
	float movingRatio = 1;


	glPushMatrix();
    
    moveCamera(&game->cameraPosition, game->selectedCharacter, movingRatio);
    glTranslatef(game->cameraPosition.x, game->cameraPosition.y, 0.);
    moveCharacter(game->selectedCharacter, characterMoving, motion, xMoving, game->obstaclesMap, game->nbObstacles);
    selectACharacter(game->availableCharacters, &game->whichCharacter, &game->selectedCharacter, game->nbCharacters);
   	reachFinalPosition(game->selectedCharacter, game->availableCharacters, &game->whichCharacter, game->nbCharacters);
    
    // Affichage des obstacles
    glColor3ub(game->obstaclesColor.red, game->obstaclesColor.green, game->obstaclesColor.blue);
    for (i = 0; i < game->nbObstacles; ++i)
      drawObstacle(game->obstaclesMap[i]);

    // Affichage des personnages et de leur position finale
    for (i = 0; i < game->nbCharacters; ++i)
    {
      drawCharacter(game->availableCharacters[i]);
      drawFinalPosition(game->availableCharacters[i]);
    }
    
  glPopMatrix();


  // Affichage des petits avatars des personnages en bas à droite
  for (i = 0; i < game->nbCharacters; ++i)
    drawAvatar(game->charactersColor[i], i, ratio);
  selectAvatar(game->whichCharacter, ratio);
}


// Charger une texture
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

  // Transmission de l'image
  glTexImage2D(GL_TEXTURE_2D, 0, 4, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
  //Type, mipmap, couleurs, largeur, hauteur, bordure, format, type des couleurs, adresse image

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);
    
  SDL_FreeSurface(image);
  
  return imageName;
}


void showImage(GLuint imageName, int full, float ratio)
{
  float xPosition = UNIT;

  if (full)
    xPosition*=ratio;

  // Activation & sélection texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, imageName);

  // Dessin
  glBegin(GL_QUADS);
    glTexCoord2f(0, 1);
    glVertex2f(-xPosition, -UNIT);

    glTexCoord2f(1, 1);
    glVertex2f(xPosition, -UNIT);

    glTexCoord2f(1, 0);
    glVertex2f(xPosition, UNIT);

    glTexCoord2f(0, 0);
    glVertex2f(-xPosition, UNIT);
  glEnd();

  // Désélection & désactivation texture
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}