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

/**
 * @file constants.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Stores constants for programs to use
 * @version 0.1
 * @date 2022-09-05
 */

#define DENSITY 1
#define VISCOSITY 1
#define FORCEMULT 0.3
/**
 * @file advection.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Calculates the advection of the fluid
 * @version 0.1
 * @date 2022-09-04
 */

/*
From the text; 

q(x, t+del t) = q(x - u(x, t) del t, t)

where 
    x is our position,
    t is time,
    del t is change in time, 
    u(x, t) is velocity at position x at time t

q is our quantity function
*/

// uses global variables 
//  velocity field u -> velTex
//  quantity field x -> qntTex
//  delta t (timestep) -> dt
//  resolution of texture -> res
void advect(vec2 coords, out vec4 xNew) {
    vec2 pos = coords - dt * (res.x / res.y) * texture(velTex, coords).xy;
    vec4 xL = texture(qntTex, pos - vec2(delx, 0));
    vec4 xR = texture(qntTex, pos + vec2(delx, 0));
    vec4 xB = texture(qntTex, pos - vec2(0, dely));
    vec4 xT = texture(qntTex, pos + vec2(0, dely));
    
    xNew = mix(mix(xL, xR, 0.5), mix(xB, xT, 0.5), 0.5);
}

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
