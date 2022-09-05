/**
 * @file difStep.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Diffusion step
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
 * @file math.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Computes various mathematical operations
 * @version 0.1
 * @date 2022-09-04
 */

// Jacobi iteration
// Poisson-pressure equation; x -> p, b -> del dot w, alpha -> -(delta x)^2, beta -> 4
// Viscous x,b -> u (velocity field), alpha = (delta x)^2/v delta t, beta -> 4 + alpha
void jacobi(vec2 coords, out vec4 xNew, float alpha, float rbeta, sampler2D x, sampler2D b) {
    vec4 xL = texture(x, coords - vec2(delx, 0));
    vec4 xR = texture(x, coords + vec2(delx, 0));
    vec4 xB = texture(x, coords - vec2(0, dely));
    vec4 xT = texture(x, coords + vec2(0, dely));

    vec4 bC = texture(b, coords);

    xNew = (xL + xR + xB + xT + alpha * bC) * rbeta;
}

// Divergence
void divergence(vec2 coords, out vec4 div, sampler2D x) {
    vec4 xL = texture(x, coords - vec2(delx, 0));
    vec4 xR = texture(x, coords + vec2(delx, 0));
    vec4 xB = texture(x, coords - vec2(0, dely));
    vec4 xT = texture(x, coords + vec2(0, dely));

    div = vec4((res.x / res.y) * 0.5 * ((xR.x - xL.x) + (xT.y - xB.y)));
}

// Gradient
void gradient(vec2 coords, out vec4 uNew, sampler2D p, sampler2D w) {
    float pL = texture(p, coords - vec2(delx, 0)).x;
    float pR = texture(p, coords + vec2(delx, 0)).x;
    float pB = texture(p, coords - vec2(0, dely)).x;
    float pT = texture(p, coords + vec2(0, dely)).x;
    
    uNew = texture(w, coords);
    uNew.xy -= (res.x / res.y) * 0.5 * vec2(pR - pL, pT - pB);
}
/**
 * @file diffusion.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Calculates the diffusion of the fluid
 * @version 0.1
 * @date 2022-09-04
 */

/*
From the text;

(I - (v)(del t)(Laplacian)) u(x, t + del t) = u (x, t)

Solved using Jacobi iterations
*/

void diffusion(vec2 coords, out vec4 xNew) {
    float alpha = delx * delx / (VISCOSITY * dt);
    float rbeta = 1 / (4 + alpha);
    for (int i = 0; i < 20; i ++) {
        jacobi(coords, xNew, alpha, rbeta, velTex, velTex);
    }
}

void main() {
    diffusion(uv, fragColor);
}
