/**
 * @file fluid.vs
 * @author Eron Ristich (eron@ristich.com)
 * @brief Vertex shader for main execution of the fluid plane. Essentially only passes the position on the screen
 * @version 0.1
 * @date 2022-09-03
 */
#version 430 core

attribute vec2 pos;
varying vec2 uv;

void main() {
    uv = pos*0.5+0.5;
    gl_Position = vec4(pos, 0, 1);
}
