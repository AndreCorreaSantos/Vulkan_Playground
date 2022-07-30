#version 450

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
void main() {

 //noisePosition in xyz

  vec3 tangent = dFdx(position);
  vec3 bitangent = dFdy(position);
  vec3 normal = max( normalize(cross(tangent,bitangent)),0.0); //testing transforming normals from model space to world space
  //they are flat shaded at the moment.
  //temporary
  //vec3 normalWorldSpace = normalize(mat3(push.modelMatrix)*normal);

  float cosTheta = dot(normal,light)+ambient;


  outColor = vec4(fragColor*cosTheta, 1.0);
}
