/**
 * @file frcStep.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Force step
 * @version 0.1
 * @date 2022-09-05
 */
#version 430 core

out vec4 fragColor;

varying vec2 uv;

uniform int frame;
uniform float dt;
uniform vec2 res; // window resolution
uniform vec2 mpos; // current mouse position
uniform vec2 rel; // relative mouse movement (in pixels)
uniform int mDown; // if 0 mouse is up, else, mouse is down

uniform sampler2D velTex; // velocity texture
uniform sampler2D intTex; // intermediate texture
uniform sampler2D tmpTex; // temporary texture
uniform sampler2D prsTex; // pressure texture
uniform sampler2D qntTex; // quantity texture

float delx = 1 / res.x;
float dely = 1 / res.y;

#include math/constants.fs
#include math/force.fs

void main() {
    vec4 temp = vec4(0);
    if (mDown != 0) {
        applyForce(uv, temp, 1);
    }
    fragColor = texture(velTex, uv) + temp;
}