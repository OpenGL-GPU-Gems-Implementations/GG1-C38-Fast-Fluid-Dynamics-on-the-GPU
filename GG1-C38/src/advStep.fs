/**
 * @file advStep.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Advection step
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
uniform sampler2D tmpTex; // temporary texture
uniform sampler2D prsTex; // pressure texture
uniform sampler2D qntTex; // quantity texture

float delx = 1 / res.x;
float dely = 1 / res.y;

#include math/constants.fs
#include math/advection.fs

void main() {
    vec4 force = vec4(0);
    if (mDown != 0) {
        vec2 orgPos = mpos / res; // original mouse position rescaled
        vec2 relMmt = rel / res; // relative mouse motion rescaled
        float dist = distance(uv, orgPos);
        float a = 0.12;
        float val = (a / (dist + a)) - 0.5;
        float frm = frame;
        if (dist < 0.15) {
            force = vec4(val*cos(frm/200), val*sin(frm/100), val*sin(frm/300), 1);
            force = abs(force);
            force *= 0.7;
        }
    }
    advect(uv, fragColor);
    fragColor += force;
    fragColor *= 0.995;
}