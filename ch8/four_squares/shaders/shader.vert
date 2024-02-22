#version 450

/* Provide by a vertex buffer */
layout(location = 0) in vec2 in_coords;
layout(location = 1) in vec3 in_color;

/* Provided by a push constant */
layout(push_constant) uniform push_block {
  vec2 points[4];
} push;

/* Provided by a descriptor set */
layout(set=0, binding=0) uniform color_offset_block {
  vec3 color_offset;
} offset;

/* Delivered to the fragment shader */
layout(location = 0) out vec3 new_color;

void main() {

  /* Set a position offset according to instance index */
  vec2 pos_offset[4] = vec2[4](push.points[0], push.points[1], 
    push.points[2], push.points[3]);
  
  /* Set the vertex coordinates */
  gl_Position = 
    vec4(in_coords + pos_offset[gl_InstanceIndex], 0.0, 1.0);

  /* Set the output color */
  new_color = in_color - 
    (offset.color_offset * gl_InstanceIndex);
}