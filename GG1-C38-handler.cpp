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

    curPrs = NULL; curVel = NULL; curQnt = NULL;
    nxtPrs = NULL; nxtVel = NULL; nxtQnt = NULL;
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

void GG1_C38_Handler::setShader(Shader* shader) {
    glm::vec2 res = glm::vec2(kernel->getRX(), kernel->getRY());
    glm::vec2 mpos = glm::vec2(orgX, res.y - orgY);
    glm::vec2 rel = glm::vec2(relX, -relY);
    int mDown = mouseDown;

    shader->use();
    
    shader->setInt("frame", frame);
    shader->setFloat("dt", dt);
    shader->setVec2("res", res);
    shader->setVec2("mpos", mpos);
    shader->setVec2("rel", rel);
    shader->setInt("mDown", mDown);

    shader->setInt("velTex", 0);
    shader->setInt("tmpTex", 1);
    shader->setInt("prsTex", 2);
    shader->setInt("qntTex", 3);
}

void GG1_C38_Handler::advectionStep() {
    setShader(advStep);
    
    glBindFramebuffer(GL_FRAMEBUFFER, nxtQnt->FBO);

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curVel->TEX);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tmp->TEX);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TexturePair* temp = nxtQnt;
    nxtQnt = curQnt;
    curQnt = temp;
}

void GG1_C38_Handler::forceStep() {
    setShader(frcStep);
    
    glBindFramebuffer(GL_FRAMEBUFFER, nxtVel->FBO);

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curVel->TEX);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tmp->TEX);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TexturePair* temp = nxtVel;
    nxtVel = curVel;
    curVel = temp;
}

void GG1_C38_Handler::diffusionStep() {
    for (int i = 0; i < 20; i ++) {
        setShader(difStep);
        
        glBindFramebuffer(GL_FRAMEBUFFER, nxtVel->FBO);

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, curVel->TEX);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tmp->TEX);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

        glBegin(GL_POLYGON);
            glVertex3f(-1, -1, 0);
            glVertex3f(-1, 1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, -1, 0);
        glEnd();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        TexturePair* temp = nxtVel;
        nxtVel = curVel;
        curVel = temp;
    }
}

void GG1_C38_Handler::divergenceStep() {
    setShader(divStep);
    
    glBindFramebuffer(GL_FRAMEBUFFER, tmp->FBO);

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curVel->TEX);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tmp->TEX);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GG1_C38_Handler::pressureStep() {
    for (int i = 0; i < 40; i ++) {
        setShader(prsStep);
        
        glBindFramebuffer(GL_FRAMEBUFFER, nxtPrs->FBO);

        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, curVel->TEX);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tmp->TEX);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

        glBegin(GL_POLYGON);
            glVertex3f(-1, -1, 0);
            glVertex3f(-1, 1, 0);
            glVertex3f(1, 1, 0);
            glVertex3f(1, -1, 0);
        glEnd();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        TexturePair* temp = nxtPrs;
        nxtPrs = curPrs;
        curPrs = temp;
    }
}

void GG1_C38_Handler::gradientStep() {
    setShader(grdStep);
    
    glBindFramebuffer(GL_FRAMEBUFFER, nxtVel->FBO);

    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curVel->TEX);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tmp->TEX);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, curQnt->TEX);

    glBegin(GL_POLYGON);
		glVertex3f(-1, -1, 0);
		glVertex3f(-1, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(1, -1, 0);
	glEnd();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    TexturePair* temp = nxtVel;
    nxtVel = curVel;
    curVel = temp;
}

void GG1_C38_Handler::objRendererHandler() {
    advectionStep();
    forceStep();
    diffusionStep();
    divergenceStep();
    pressureStep();
    gradientStep();

    glClear(GL_COLOR_BUFFER_BIT);
    setShader(fluidShader);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, curVel->TEX);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tmp->TEX);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, curPrs->TEX);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, curQnt->TEX);
    
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
    
    // sets the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // setup FBO's
    int rx = kernel->getRX();
    int ry = kernel->getRY();
    vel1 = new TexturePair(rx, ry);
    vel2 = new TexturePair(rx, ry);
    tmp = new TexturePair(rx, ry);
    qnt1 = new TexturePair(rx, ry);
    qnt2 = new TexturePair(rx, ry);
    prs1 = new TexturePair(rx, ry);
    prs2 = new TexturePair(rx, ry);

    curVel = vel1; nxtVel = vel2;
    curQnt = qnt1; nxtQnt = qnt2;
    curPrs = prs1; nxtPrs = prs2;

    // setup fluid shaders
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