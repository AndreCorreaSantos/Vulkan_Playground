#version 450

layout(location = 0) in vec3 position; //xyz must be outputed as xzy
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec3 outPosition; //sending position with noise to fragment shader in xyz

float N21(vec2 p) {
    return fract(sin(p.x*100.+p.y*6574.)*5647.);
}

float SmoothNoise(vec2 uv) {
    vec2 lv = fract(uv);
    vec2 id = floor(uv);
    
    lv = lv*lv*(3.-2.*lv);
    
    float bl = N21(id);
    float br = N21(id+vec2(1,0));
    float b = mix(bl, br, lv.x);
    
    float tl = N21(id+vec2(0,1));
    float tr = N21(id+vec2(1,1));
    float t = mix(tl, tr, lv.x);
    
    return mix(b, t, lv.y);
}

float SmoothNoise2(vec2 uv) {
    float c = 0;//SmoothNoise(uv*4.);
    float dist = clamp(length(uv),1,1000000);
    // don't make octaves exactly twice as small
    // this way the pattern will look more random and repeat less
    c += SmoothNoise(uv/2)*10;
    
    //c += SmoothNoise(uv*8.2)*0.005;
    
    // c += SmoothNoise(uv*16.7)*.025;
    // c += SmoothNoise(uv*32.4)*.015;
    //c += SmoothNoise(uv*64.5)*.0625;
    
    
    return c;
}



layout(push_constant) uniform Push {
  mat4 transform; //projection*view*model
  mat4 modelMatrix;
  float time;
} push;

vec3 noisePosition;
vec2 center = vec2(15.0,15.0);
void main() {
  //removing time to test lighting
  //float dist = 1;//(1/-clamp( length(center-vec2(position[0],position[1])),1.0000,100000.0 ))-0.1;

  //loat scaling = 15;

  float z = SmoothNoise2(vec2((position[0]),(position[1])));
  noisePosition = vec3(position[0],z,position[1]);
  gl_Position = push.transform*vec4(noisePosition,1.0);

  fragColor = color;
  outPosition = vec3(noisePosition[0],noisePosition[2],noisePosition[1]);
}
