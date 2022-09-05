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

class GG1_C38_Handler : public Handler {
    public:
        GG1_C38_Handler();
        ~GG1_C38_Handler();

        void objEventHandler() override;
        void objRendererHandler() override;
        void objUpdateHandler() override;
        void objPreLoopStep() override;

    private:
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
        GLuint velFBO, intFBO, tmpFBO, qntFBO, prsFBO;
        GLuint velTex, intTex, tmpTex, qntTex, prsTex;

        Shader* fluidShader;

};

#endif


