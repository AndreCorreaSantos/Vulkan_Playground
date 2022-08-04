#version 450

precision highp float;
precision highp int;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 inPos;

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
  mat4 transform;
  mat4 modelMatrix;
  float time;
} push;

void main() {
  vec3 color = vec3(fragColor[0],fragColor[1],fragColor[2]);
  outColor = vec4(color, 1.0);
}
