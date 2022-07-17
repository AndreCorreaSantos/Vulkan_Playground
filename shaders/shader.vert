#version 450


layout(location = 0) in vec2 position;
layout(location = 0) out vec3 outColor;

const vec3 colors[3] = vec3[3](
	vec3(1.0f, 0.0f, 0.0f), //red
	vec3(0.0f, 1.0f, 0.0f), //green
	vec3(00.f, 0.0f, 1.0f)  //blue
);
int index = 0;

void main() {

  gl_Position = vec4(position, 0.0, 1.0);
  outColor =  colors[gl_VertexIndex];
  index += 1;
} 