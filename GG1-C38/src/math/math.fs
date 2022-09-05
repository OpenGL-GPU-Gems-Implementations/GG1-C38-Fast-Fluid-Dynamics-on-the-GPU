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