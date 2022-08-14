#version 450

layout(location = 0) in vec3 position; //xyz must be outputed as xzy
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 outPos;

layout(push_constant) uniform Push {
  mat4 transform; //projection*view*model
  mat4 modelMatrix;
  float time;
} push;


void main() {

  vec3 xzyPosition = vec3(position[0],position[2],position[1]);
  gl_Position = push.transform*vec4(xzyPosition,1.0);
  outPos = position;
  fragColor = color;
}
