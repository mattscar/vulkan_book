#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

/* Per-vertex data */
layout(location = 0) in vec3 in_coords;
layout(location = 1) in vec3 in_color;
layout(location = 2) in vec3 in_normal;

/* Uniform data */
layout(binding=0) uniform uniform_data {
  mat4 model_matrix;
  mat4 view_matrix;
  mat4 projection_matrix;
  vec3 viewer_pos;
  vec3 light_pos;
} uniforms;

/* Passed to the fragment shader */
layout(location = 0) out vec3 out_color;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec3 out_viewer_direction;
layout(location = 3) out vec3 out_light_direction;

void main() {

  /* Apply the model transformation */
  vec4 world_coords =
    uniforms.model_matrix * vec4(in_coords, 1.0);

  /* Translate the second and third instances */
  world_coords.z = world_coords.z - gl_InstanceIndex * 5;

  /* Apply the view transformation */
  vec4 eye_coords = 
    uniforms.view_matrix * world_coords;
  
  /* Determine the direction to the viewer */
  out_viewer_direction = 
    normalize(uniforms.viewer_pos - eye_coords.xyz);
  
  /* Apply the projection transformation */
  vec4 clip_coords =
    uniforms.projection_matrix * eye_coords;

  /* Determine the direction to the light source */
  out_light_direction = 
    normalize(uniforms.light_pos - clip_coords.xyz);

  /* Set output variables */
  out_color = in_color;
  out_normal = in_normal;
  gl_Position = clip_coords;  
}