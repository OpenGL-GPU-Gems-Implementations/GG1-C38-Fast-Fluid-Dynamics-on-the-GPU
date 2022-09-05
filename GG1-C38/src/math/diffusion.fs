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