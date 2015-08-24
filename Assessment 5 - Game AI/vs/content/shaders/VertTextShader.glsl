#version 330
 
//shader from:
// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01/

attribute vec4 coord;
varying vec2 texcoord;

uniform mat4 MVP; 
 
void main(void) {
  vec4 oldPos =  vec4(coord.xy, 0, 1);
  texcoord = coord.zw;
  
  vec4 newPos = MVP * oldPos;
  
  gl_Position = newPos;
}