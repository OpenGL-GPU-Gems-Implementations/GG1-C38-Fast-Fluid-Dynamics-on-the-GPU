/**
 * @file GG1-C38-handler.h
 * @author Eron Ristich (eron@ristich.com)
 * @brief Handler class for GG1-C38
 * @version 0.1
 * @date 2022-09-03
 */

#ifndef GG1_C38_HANDLER_H
#define GG1_C38_HANDLER_H

#include <chrono>
#include <cstdarg>
#include <string>
using std::string;

#include "util/handler.h"
#include "util/glslInclude.h"
#include "objects/helper.h"

class TexturePair {
    public:
        TexturePair(int rx, int ry) {
            FBO = 0; TEX = 0;
            setupFBO(rx, ry);
        }

        GLuint FBO, TEX;
    private:
        void setupFBO(int rx, int ry) {
            cout << "setup: ";
            glGenFramebuffers(1, &FBO);

            glGenTextures(1, &TEX);
            glBindTexture(GL_TEXTURE_2D, TEX);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, rx, ry, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
            
            glBindFramebuffer(GL_FRAMEBUFFER, FBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEX, 0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0);
            //GLuint clearColor[4] = {0, 0, 0, 0};
            //glClearBufferuiv(GL_COLOR, 0, clearColor);
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE) {
                cout << TEX << " " << FBO << "\n";
            } else {
                cout << "nay\n";
            }
            
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
};

class GG1_C38_Handler : public Handler {
    public:
        GG1_C38_Handler();
        ~GG1_C38_Handler();

        void objEventHandler() override;
        void objRendererHandler() override;
        void objUpdateHandler() override;
        void objPreLoopStep() override;

    private:
        void setShader(Shader* shader);
        void advectionStep();
        void forceStep();
        void diffusionStep();
        void divergenceStep();
        void pressureStep();
        void gradientStep();

        int frame = 0;
        float dt = 0.0f;
        int curFPS = 0;
        std::chrono::_V2::steady_clock::time_point lastT;

        int relX, relY, orgX, orgY;
        bool wDown, aDown, sDown, dDown, spDown, shDown, enDown;
        bool mouseDown;
        
        // scene objects
        /* ----- FLUID PLANE ----- */
        Shader *advStep, *frcStep, *difStep, *divStep, *prsStep, *grdStep;
        TexturePair *vel1, *vel2, *tmp, *qnt1, *qnt2, *prs1, *prs2;
        TexturePair *curVel, *nxtVel, *curQnt, *nxtQnt, *curPrs, *nxtPrs;
        
        Shader* fluidShader;

};

#endif


