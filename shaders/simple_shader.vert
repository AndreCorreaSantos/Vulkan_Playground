#version 450

layout(location = 0) in vec3 position; //xyz must be outputed as xzy
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;


float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 p, float freq ){
  float PI = 3.1415926535;
  float screenWidth = 2000.0;
	float unit = screenWidth/freq;
	vec2 ij = floor(p/unit);
	vec2 xy = mod(p,unit)/unit;
	//xy = 3.*xy*xy-2.*xy*xy*xy;
	xy = .5*(1.-cos(PI*xy));
	float a = rand((ij+vec2(0.,0.)));
	float b = rand((ij+vec2(1.,0.)));
	float c = rand((ij+vec2(0.,1.)));
	float d = rand((ij+vec2(1.,1.)));
	float x1 = mix(a, b, xy.x);
	float x2 = mix(c, d, xy.x);
	return mix(x1, x2, xy.y);
}


layout(push_constant) uniform Push {
  mat4 transform;
  vec3 color;
  float time;
} push;

void main() {
  float time = push.time;
  vec2 xy = vec2(position[0]+time,position[1]+time);

  float frequency = 50.0;
  float z = sin(position[0]+time)*cos(position[1]+time);
  vec3 noisePosition = vec3(position[0],z,position[1]);
  gl_Position = push.transform*vec4(noisePosition,1.0);
  fragColor = color;
}
