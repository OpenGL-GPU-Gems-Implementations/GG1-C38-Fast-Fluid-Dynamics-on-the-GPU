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
uniform sampler2D intTex; // intermediate texture
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
#define FORCEMULT 0.5
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
    advect(uv, fragColor);
}
