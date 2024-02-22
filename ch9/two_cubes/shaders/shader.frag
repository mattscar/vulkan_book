#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 in_color;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_viewer_direction;
layout(location = 3) in vec3 in_light_direction;

/* Uniform data */
layout(binding=1) uniform uniform_data {
  vec3 light_params[3];
  float shininess;
} uniforms;

/* Output color */
layout(location = 0) out vec4 out_color;

void main() {

  /* Step 1: Compute N . L */
  float n_dot_l = dot(in_normal, in_light_direction);

  /* Step 2: Compute H, the vector between L and V */
  vec3 half_vector =
    normalize(in_light_direction + in_viewer_direction);

  /* Step 3: Compute (N . H)^n' */
  float blinn = dot(in_normal, half_vector);
  blinn = clamp(blinn, 0.0f, 1.0f);
  blinn = pow(blinn, uniforms.shininess);

  /* Step 4: Compute sum of light components */
  vec3 light_color = uniforms.light_params[0] +
    uniforms.light_params[1] * n_dot_l +
    uniforms.light_params[2] * blinn;
  light_color = clamp(light_color, 0.0f, 1.0f);

  /* Step 5: Blend light color and original color */
  vec3 color_sum = clamp((light_color + in_color)/2.0f, 0.0f, 1.0f);
  out_color = vec4(color_sum, 1.0);
}