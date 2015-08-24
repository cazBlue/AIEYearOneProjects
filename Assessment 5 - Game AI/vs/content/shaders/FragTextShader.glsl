#version 120
 
//shader from:
// http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Text_Rendering_01/

varying vec2 texcoord;
uniform sampler2D tex;
uniform vec4 color;
 
void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texcoord).a) * color;
}