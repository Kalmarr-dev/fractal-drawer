#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform vec4 u_camera;


void main() {
  vec4 newPosition = position;
  newPosition.xy -= u_camera.xy;
  newPosition.xy /= u_camera.zw / vec2(2.0, 2.0);
  newPosition.xy -= vec2(1.0, 1.0);
  gl_Position = newPosition;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main() {
  color = u_color;
}
