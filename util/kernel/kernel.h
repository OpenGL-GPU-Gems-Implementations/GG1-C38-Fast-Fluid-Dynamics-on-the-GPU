/**
 * @file kernel.h
 * @author Eron Ristich (eron@ristich.com)
 * @brief Generic control for a single SDL window with OpenGL
 * @version 0.1
 * @date 2022-07-19
 */

#ifndef KERNEL_H
#define KERNEL_H

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Kernel {
    public:
        Kernel(string title, int rx, int ry);
        ~Kernel();

        // renders to the screen based off of a function that draws all objects on the screen (independently of the kernel object)
        void render();

        // registers an event handler for events that occur in SDL window
        bool registerEventHandler(void (*f)());
        bool registerRendererHandler(void (*f)());
        bool registerUpdateHandler(void (*f)());

        // A function that runs immediately before the start loop
        bool registerPreLoopStep(void (*f)());

        // starts window render loop
        void start();
        
        // stops window render loop
        void stop();

        // getter functions
        string getTitle();
        int getRX();
        int getRY();
        SDL_Window* getWindow();
        SDL_Renderer* getRenderer();

        // save window as image
        bool saveImage(SDL_Window* window, SDL_Renderer* renderer, const char* file);

    private:
        SDL_Window* createWindow(string title, int rx, int ry);
        SDL_Renderer* createRenderer(SDL_Window* wind);

        bool initSDL();
        bool initGL();
        bool initIMG();

        Uint32 getPixel(SDL_Surface* surface, int x, int y);
        void flipSurface(SDL_Surface* surface);

        string title;
        int rx, ry;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_GLContext glContext;
        
        void (*eventHandler)() = NULL;
        void (*rendererHandler)() = NULL;
        void (*updateHandler)() = NULL;
        void (*preLoopStep)() = NULL;

        bool running = false;
};

#endif