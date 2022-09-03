/**
 * @file GG1-C38-handler.cpp
 * @author Eron Ristich (eron@ristich.com)
 * @brief Handler class for the display case 
 * @version 0.1
 * @date 2022-09-03
 */

#include "GG1-C38-handler.h"

GG1_C38_Handler::GG1_C38_Handler() {
    wDown = false; aDown = false; sDown = false; dDown = false; spDown = false; shDown = false; enDown = false;
    mouseDown = false;
}

GG1_C38_Handler::~GG1_C38_Handler() {
    
}

void GG1_C38_Handler::objEventHandler() {
    relX = 0; relY = 0;
    SDL_Event m_event;
	while(SDL_PollEvent(&m_event)) {
		switch (m_event.type) {
            case SDL_KEYDOWN:
                switch (m_event.key.keysym.sym) {
                    case SDLK_ESCAPE: // exit window
                        kernel->saveImage(kernel->getWindow(), kernel->getRenderer(), "renders/fluid.bmp");
                        kernel->stop();
                        break;
                    case SDLK_w: // w
                        wDown = true;
                        break;
                    case SDLK_a: // a
                        aDown = true;
                        break;
                    case SDLK_s: // s
                        sDown = true;
                        break;
                    case SDLK_d: // d
                        dDown = true;
                        break;
                    case SDLK_SPACE: // spacebar
                        spDown = true;
                        break;
                    case SDLK_LSHIFT: // left shift
                        shDown = true;
                        break;
                    case SDLK_RETURN: // enter
                        enDown = true;
                        break;
                }
                break;
            
            case SDL_KEYUP:
                switch (m_event.key.keysym.sym) {
                    case SDLK_w: // w
                        wDown = false;
                        break;
                    case SDLK_a: // a
                        aDown = false;
                        break;
                    case SDLK_s: // s
                        sDown = false;
                        break;
                    case SDLK_d: // d
                        dDown = false;
                        break;
                    case SDLK_SPACE: // spacebar
                        spDown = false;
                        break;
                    case SDLK_LSHIFT: // left shift
                        shDown = false;
                        break;
                    case SDLK_RETURN: // enter
                        enDown = false;
                        break;
                }
                break;

            case SDL_WINDOWEVENT:
                switch (m_event.window.event) {
                    case SDL_WINDOWEVENT_CLOSE: // exit window
                        kernel->stop();
                        break;
                }
                break;
            
            case SDL_MOUSEMOTION:
                relX = m_event.motion.xrel;
                relY = m_event.motion.yrel;
                orgX = m_event.motion.x - relX;
                orgY = m_event.motion.y - relY; 
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                mouseDown = true;
                break;
            
            case SDL_MOUSEBUTTONUP:
                mouseDown = false;
                break;
        }
	}
}

void GG1_C38_Handler::objRendererHandler() {
    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();
}

void GG1_C38_Handler::objUpdateHandler() {
    // update time
    frame ++;
    auto curT = std::chrono::steady_clock::now();
    std::chrono::duration<float> diff = curT - lastT;
    lastT = std::chrono::steady_clock::now();
    dt = diff.count();
    curFPS = (int)(1/dt);

    // update title
    string atitle = kernel->getTitle() + string(" - FPS: ") + std::to_string(curFPS) + string(" - Frame: ") + std::to_string(frame);
    SDL_SetWindowTitle(kernel->getWindow(), atitle.c_str());
}

void GG1_C38_Handler::objPreLoopStep() {
    lastT = std::chrono::steady_clock::now();
    
    // sets the clear color to black, sets relative mouse mode to true, and enables GL_DEPTH_TEST.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    glEnable(GL_DEPTH_TEST);

    // setup fluid shader

}