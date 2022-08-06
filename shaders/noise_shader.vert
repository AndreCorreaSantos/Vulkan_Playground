#version 450

layout(location = 0) in vec3 position; //xyz must be outputed as xzy
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 outPosition; //sending position with noise to fragment shader in xyz


layout(push_constant) uniform Push {
  mat4 transform; //projection*view*model
  mat4 modelMatrix;
  float time;
} push;


vec3 permute(vec3 x) { return mod(((x*34.0)+1.0)*x, 289.0); }

float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, 0.366025403784439,
           -0.577350269189626, 0.024390243902439);
  vec2 i  = floor(v + dot(v, C.yy) );
  vec2 x0 = v -   i + dot(i, C.xx);
  vec2 i1;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  i = mod(i, 289.0);
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
  + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy),
    dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  vec3 g;
  g.x  = a0.x  * x0.x  + h.x  * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


float gln_PI = 3.1415;

struct gln_tGerstnerWaveOpts {
    vec2 direction;   // Direction of the wave
    float steepness;  // Steepness/Sharpness of the peaks
    float wavelength; // Wavelength...self explnitory
};

vec3 gln_GerstnerWave(vec3 p, gln_tGerstnerWaveOpts opts, float time) {
    float steepness = opts.steepness;
    float wavelength = opts.wavelength;

    float k = 2.0 * gln_PI / wavelength;
    float c = sqrt(9.8 / k);
    vec2 d = normalize(opts.direction);
    float f = k * (dot(d, p.xy) - c * time);
    float a = steepness / k;

    return vec3(
        d.x * (a * cos(f)),
        a * sin(f),
        d.y * (a * cos(f))
    );
}
gln_tGerstnerWaveOpts A = gln_tGerstnerWaveOpts(vec2(0.0, -1.0), 0.5, 2.0);
gln_tGerstnerWaveOpts B = gln_tGerstnerWaveOpts(vec2(0.0, 1.0), 0.25, 4.0);
gln_tGerstnerWaveOpts C = gln_tGerstnerWaveOpts(vec2(1.0, 1.0), 0.15, 6.0);
gln_tGerstnerWaveOpts D = gln_tGerstnerWaveOpts(vec2(1.0, 1.0), 0.4, 2.0);

vec3 displace(vec3 point) {
  vec3 n = vec3(0.0);
  vec3 p = point;
  vec3 pt = vec3(p.x,p.y+push.time*1.5,p.z);

  n += snoise(pt.xy)*0.04;
  n += gln_GerstnerWave(pt, A, 1.0).xzy;
  n += gln_GerstnerWave(p, B, 1.0).xzy * 0.5;
  n += gln_GerstnerWave(p, C, 1.0).xzy * 0.25;
  n += gln_GerstnerWave(p, D, 1.0).xzy * 0.2;
  return point + n;
}

vec2 center = vec2(0,0);
void main() {

  vec3 noisePosition = displace(vec3(position[0],position[1],position[2]));

  gl_Position = push.transform*vec4(vec3(noisePosition[0],-noisePosition[2],noisePosition[1]),1.0);

  fragColor = color;
  outPosition = vec3(noisePosition[0],noisePosition[2],noisePosition[1]);
}
