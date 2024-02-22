#version 450

layout(location = 0) in vec2 in_coords;
layout(location = 1) in vec3 in_color;
layout(location = 0) out vec3 new_color;

void main() {
  gl_Position = vec4(in_coords, 0.0, 1.0);
  new_color = in_color;
}
