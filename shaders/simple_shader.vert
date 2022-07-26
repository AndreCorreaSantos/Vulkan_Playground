#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;

layout(push_constant) uniform Push {
  mat4 transform;
  vec3 color;
} push;

void main() {
  gl_Position = push.transform*vec4(position,1.0);
  fragColor = color;
  if( (position[1]< 1.0)&&(position[1]> -1.0) )
  {
    fragColor = vec3(0.5, 0.35, 0.05);
  } else if( position[1] < -12.0) {
    fragColor = vec3(0.8,0.8,0.8);
  }
}
