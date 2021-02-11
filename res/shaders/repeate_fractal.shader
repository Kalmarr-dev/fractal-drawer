#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform vec4 u_camera;
uniform mat2 u_scaleRotationMatrix;
uniform vec4 u_rotationOrigin;
uniform vec4 u_mainLine;


void main() {
  vec2 pointFromOrigin = position.xy - u_rotationOrigin.xy;
  vec2 modifiedPointFromOrigin = u_scaleRotationMatrix * pointFromOrigin;
  vec4 newPosition = position;
  newPosition.xy = modifiedPointFromOrigin + u_mainLine.xy;
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
