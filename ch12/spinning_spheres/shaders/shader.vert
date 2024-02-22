#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

/* Per-vertex data */
layout(location = 0) in vec3 in_coords;

/* Color */
layout(location = 0) out vec4 out_color;

/* Provided by push constant */
layout(push_constant) uniform push_block {
  mat4 mvpMatrix;
  vec4 color;
} push;

void main() {

  /* Set output variables */
  gl_Position = push.mvpMatrix * vec4(in_coords, 1.0);
  out_color = push.color;
}