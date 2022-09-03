/**
 * @file skybox.h
 * @author Eron Ristich (eron@ristich.com)
 * @brief Skybox vertices and VAO/VBO setup. Outsourced primarily for organization
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef SKYBOX_H
#define SKYBOX_H

#include <string>

#include "helper.h"

static float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
    1.0f,  1.0f, -1.0f,
    1.0f,  1.0f,  1.0f,
    1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f, -1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
    1.0f, -1.0f,  1.0f
};

/**
 * @brief Manages skybox vertices and VAO/VBO setup. Also stores shader information.
 */
class Skybox {
    public:
        unsigned int skyboxVAO, skyboxVBO, cubeTexture;
        Shader* shader;

        // Requires vertex path, fragment path, as well as paths to each face of the skybox in the order right, left, top, bottom, front, back or +x, -x, +y, -y, +z, -z
        Skybox(const char* vertexPath, const char* fragmentPath, vector<std::string> faces) {
            shader = new Shader(vertexPath, fragmentPath);
            cubeTexture = loadCubemap(faces);

            glGenVertexArrays(1, &skyboxVAO);
            glGenBuffers(1, &skyboxVBO);
            glBindVertexArray(skyboxVAO);
            glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        }

        // Draws the cube (with z-depth 1, so it should always be in the back of the scene)
        void draw(Camera* camera, int rx, int ry) {
            // compute matrices
            glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), (float)rx / (float)ry, 0.1f, 100.0f);
            glm::mat4 view = camera->getViewMatrix();
            
            shader->use();
            shader->setInt("skybox", 0);

            glDepthFunc(GL_LEQUAL);
            view = glm::mat4(glm::mat3(camera->getViewMatrix()));
            shader->setMat4("view", view);
            shader->setMat4("projection", projection);
            
            glBindVertexArray(skyboxVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTexture);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            glDepthFunc(GL_LESS);
        }

};

#endif