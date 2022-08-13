#version 450

precision highp float;
precision highp int;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 position; //received in xyz

layout (location = 0) out vec4 outColor;

layout(push_constant) uniform Push {
  mat4 transform;
  mat4 modelMatrix;
  float time;
} push;

vec3 light = normalize(vec3(1.0,3.0,1.0)); //specified in xyz
const float ambient = 0.02;
float cosTheta;
vec3 deepColor = vec3(0.05,0.07,0.86);
void main() {
  float height = position[1]+0.7;
  //vec3 color = mix(deepColor,fragColor,height);
 //noisePosition in xyz
  vec3 tangent = dFdx(position);
  vec3 bitangent = dFdy(position);
  vec3 normal = max( normalize(cross(tangent,bitangent)),0.0);  //they are flat shaded at the moment.
  cosTheta = dot(normal,light)+ambient;
  outColor = vec4(fragColor*cosTheta, 1.0);
}
