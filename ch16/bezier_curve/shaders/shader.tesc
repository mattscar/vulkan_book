#version 450

#extension GL_ARB_tessellation_shader: enable

layout(vertices=4) out;

/* Provided by a push constant */
layout(push_constant) uniform push_block {
  vec2 tess_levels;
} push;

layout(location = 0) in vec3 input_points[];
layout(location = 0) out vec3 output_points[];

void main() {

  /* Set position of output control point */
  output_points[gl_InvocationID] = 
    input_points[gl_InvocationID];
  
  /* Set tessellation levels */
  gl_TessLevelOuter[0] = push.tess_levels.x;
  gl_TessLevelOuter[1] = push.tess_levels.y;
}