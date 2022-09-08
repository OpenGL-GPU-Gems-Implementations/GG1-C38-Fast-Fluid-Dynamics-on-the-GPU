/**
 * @file force.fs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Calculates the force applied at a point of the fluid
 * @version 0.1
 * @date 2022-09-04
 */

void applyForce(vec2 coords, out vec4 force, float r) {
    vec2 orgPos = mpos / res; // original mouse position rescaled
    vec2 relMmt = rel / res; // relative mouse motion rescaled

    vec2 F = relMmt * FORCEMULT;

    force = vec4(F*1/distance(coords, orgPos), 0, 0);
    //force = vec4(F*exp(pow(distance(coords, orgPos),2) / r) * dt, 0, 0);
}