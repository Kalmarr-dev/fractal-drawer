#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main() {
   gl_Position = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

// uniform vec4 u_MousePos;

void main() {
  vec2 resolution = vec2(960, 960);
  vec2 vUV = gl_FragCoord.xy/resolution.xy;

  color;
  color.r = 0.0f;
  color.g = 0.0f;
  color.b = 0.0f;
  color.b += ((0.5f + vUV.y) + (0.5f - vUV.x)) / 2;
  color.r += 0.5 + (vUV.x - vUV.y) / 2;
  color.g += 1 - ((vUV.x - 0.5f) * (vUV.x - 0.5f) * 2 + (vUV.y - 0.5f) * (vUV.y - 0.5f) * 2) / 1.5;
}
