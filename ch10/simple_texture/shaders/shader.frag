#version 450

#extension GL_ARB_separate_shader_objects: enable
#extension GL_ARB_shading_language_420pack: enable

layout(binding=0) uniform sampler2D tex_sampler;

layout(location=0) in vec2 in_tex_coords;
layout(location=0) out vec4 out_color;

void main() {
  out_color = texture(tex_sampler, in_tex_coords);
}