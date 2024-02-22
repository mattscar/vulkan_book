#version 450

/* Provide by a vertex buffer */
layout(location = 0) in vec2 in_coords;
layout(location = 1) in vec2 in_tex_coords;

/* Delivered to the fragment shader */
layout(location = 0) out vec2 out_tex_coords;

void main() {

  /* Set the vertex coordinates */
  gl_Position = vec4(in_coords, 0.0, 1.0);

  /* Set the output color */
  out_tex_coords = in_tex_coords;
}