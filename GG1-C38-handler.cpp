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

void setShaders(Shader* shader, int frame, float dt, glm::vec2 res, glm::vec2 mpos, glm::vec2 rel, int mDown) {
    shader->use();
    shader->setInt("frame", frame);
    shader->setFloat("dt", dt);
    shader->setVec2("res", res);
    shader->setVec2("mpos", mpos);
    shader->setVec2("rel", rel);
    shader->setInt("mDown", mDown);
}

void useShaders(Shader* shader, GLuint FBO, GLuint velTex, GLuint intTex, GLuint tmpTex, GLuint prsTex, GLuint qntTex) {
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    shader->use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, velTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, intTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, tmpTex);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, prsTex);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, qntTex);

    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GG1_C38_Handler::objRendererHandler() {
    glm::vec2 res = glm::vec2(kernel->getRX(), kernel->getRY());
    glm::vec2 mpos = glm::vec2(orgX, res.y - orgY);
    glm::vec2 rel = glm::vec2(relX, -relY);
    int mDown = mouseDown;
    setShaders(advStep, frame, dt, res, mpos, rel, mDown);
    setShaders(frcStep, frame, dt, res, mpos, rel, mDown);
    setShaders(difStep, frame, dt, res, mpos, rel, mDown);
    setShaders(divStep, frame, dt, res, mpos, rel, mDown);
    setShaders(prsStep, frame, dt, res, mpos, rel, mDown);
    setShaders(grdStep, frame, dt, res, mpos, rel, mDown);
    
    setShaders(fluidShader, frame, dt, res, mpos, rel, mDown);

    useShaders(advStep, qntFBO, velTex, intTex, tmpTex, prsTex, qntTex);
    useShaders(frcStep, velFBO, velTex, intTex, tmpTex, prsTex, qntTex);
    useShaders(difStep, intFBO, velTex, intTex, tmpTex, prsTex, qntTex);
    useShaders(divStep, tmpFBO, velTex, intTex, tmpTex, prsTex, qntTex);

    glBindFramebuffer(GL_FRAMEBUFFER, prsFBO);
    GLuint clearColor[4] = {0, 0, 0, 0};
    glClearBufferuiv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    useShaders(prsStep, prsFBO, velTex, intTex, tmpTex, prsTex, qntTex);
    //useShaders(grdStep, velFBO, velTex, intTex, tmpTex, prsTex, qntTex);

    glClear(GL_COLOR_BUFFER_BIT);
    fluidShader->use();
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

void setupFBO(GLuint& FBO, GLuint& TEX, int rx, int ry) {
    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &TEX);
    glBindTexture(GL_TEXTURE_2D, TEX);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rx, ry, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);
    GLuint clearColor[4] = {0, 0, 0, 0};
    glClearBufferuiv(GL_COLOR, 0, clearColor);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GG1_C38_Handler::objPreLoopStep() {
    lastT = std::chrono::steady_clock::now();
    
    // sets the clear color to black
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);

    // setup FBO's
    int rx = kernel->getRX();
    int ry = kernel->getRY();
    setupFBO(velFBO, velTex, rx, ry);
    setupFBO(intFBO, intTex, rx, ry);
    setupFBO(tmpFBO, tmpTex, rx, ry);
    setupFBO(qntFBO, qntTex, rx, ry);
    setupFBO(prsFBO, prsTex, rx, ry);

    // setup fluid shader
    string compilePath = "GG1-C38/compiled";
    string shaderVS = compileGLSL("GG1-C38/src/fluid.vs", compilePath);
    string shaderFS = compileGLSL("GG1-C38/src/fluid.fs", compilePath);
    
    string advFS = compileGLSL("GG1-C38/src/advStep.fs", compilePath);
    string frcFS = compileGLSL("GG1-C38/src/frcStep.fs", compilePath);
    string difFS = compileGLSL("GG1-C38/src/difStep.fs", compilePath);
    string divFS = compileGLSL("GG1-C38/src/divStep.fs", compilePath);
    string prsFS = compileGLSL("GG1-C38/src/prsStep.fs", compilePath);
    string grdFS = compileGLSL("GG1-C38/src/grdStep.fs", compilePath);
    
    advStep = new Shader(shaderVS.c_str(), advFS.c_str());
    frcStep = new Shader(shaderVS.c_str(), frcFS.c_str());
    difStep = new Shader(shaderVS.c_str(), difFS.c_str());
    divStep = new Shader(shaderVS.c_str(), divFS.c_str());
    prsStep = new Shader(shaderVS.c_str(), prsFS.c_str());
    grdStep = new Shader(shaderVS.c_str(), grdFS.c_str());

    fluidShader = new Shader(shaderVS.c_str(), shaderFS.c_str());
}