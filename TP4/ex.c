#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <GL/gl.h>
#include <GL/glu.h>



int main(int argc, char **argv){
  
    if(argc != 2){
      printf("usage : %s image.jpg\n",argv[0]);
      return EXIT_FAILURE;
    }
    
    SDL_Init(SDL_INIT_VIDEO);
    
    unsigned int windowWidth  = 800;
    unsigned int windowHeight = 600;
    SDL_SetVideoMode(windowWidth, windowHeight, 32, SDL_OPENGL);
    float angle = 0.;
       
    SDL_Surface* image = IMG_Load(argv[1]);
    if(image == NULL) {
        fprintf(stderr, "Impossible de charger l'image %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    GLenum format;
    switch(image->format->BytesPerPixel) {
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
            fprintf(stderr, "Format des pixels de l'image %s non pris en charge\n", argv[1]);
            return EXIT_FAILURE;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);
    
    glBindTexture(GL_TEXTURE_2D, 0);
    
    SDL_FreeSurface(image);
    
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-2, 2, -2.0*windowHeight/(float)windowWidth, 2.0*windowHeight/(float)windowWidth);
    
    int loop = 1;
    while(loop) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glPushMatrix();
            glRotatef(angle, 0., 0., 1.);

            glBegin(GL_QUADS);
            
            glColor3f(1, 1, 1);
            
            glTexCoord2f(0, 1);
            glVertex2f(-0.5, -0.5);
            
            glTexCoord2f(1, 1);
            glVertex2f(0.5, -0.5);
            
            glTexCoord2f(1, 0);
            glVertex2f(0.5, 0.5);
            
            glTexCoord2f(0, 0);
            glVertex2f(-0.5, 0.5);
            
            glEnd();
        glPopMatrix();

        angle += 5.;
        
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
        
        SDL_GL_SwapBuffers();
        
        SDL_Event e;
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                loop = 0;
            }
        }
    }
    
    glDeleteTextures(1, &textureId);
    
    SDL_Quit();

    return EXIT_SUCCESS;
}
