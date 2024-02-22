#include "pipeline_utils.h"

/* Read shader code from an SPIR-V file */
uint32_t read_binary_file(const char* filename, char** code) {
  FILE *file;
  uint32_t len;

  /* Open file containing compiled shader code */
  file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Unable to open file %s", filename);
    return 0;
  }

  /* Determine file size */
  fseek(file, 0, SEEK_END);
  len = ftell(file);
  fseek(file, 0, SEEK_SET);

  /* Allocate buffer and read code */
  *code = (char*)malloc(len);
  fread(*code, 1, len, file);

  /* Close file and return length */
  fclose(file);
  return len;
}

/* Create and configure a graphical pipeline */
void create_pipeline(Context* ctx) {

  uint32_t compute_shader_length;

  /* Read shader data */
  compute_shader_length = read_binary_file("shaders/comp.spv", &ctx->compute_shader_code);
  
  /* Pipeline layout data */
  VkPipelineLayoutCreateInfo pipeline_layout_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 1,
    .pSetLayouts = &ctx->descriptor_set_layout,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL
  };

  /* Create the pipeline layout */
  ck(vkCreatePipelineLayout(ctx->device, &pipeline_layout_info, NULL, &ctx->pipeline_layout));
  
  /* Create vertex shader module */
  VkShaderModuleCreateInfo compute_shader_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = compute_shader_length,
    .pCode = (uint32_t*)(ctx->compute_shader_code)
  };
  ck(vkCreateShaderModule(ctx->device, &compute_shader_create_info, NULL, &ctx->compute_module));
  
  /* Shader configuration info */
  VkPipelineShaderStageCreateInfo shader_stage_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stage = VK_SHADER_STAGE_COMPUTE_BIT,
    .module = ctx->compute_module,
    .pName = "main",
    .pSpecializationInfo = NULL
  };
  
  /* Pipeline configuration info */
  VkComputePipelineCreateInfo pipeline_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stage = shader_stage_info,
    .layout = ctx->pipeline_layout,
    .basePipelineHandle = NULL,
    .basePipelineIndex = -1
  };
  
  /* Create the graphics pipeline */
  vkCreateComputePipelines(ctx->device, NULL, 1, &pipeline_create_info, NULL, &ctx->pipeline);
}

/* Deallocate resources */
void destroy_pipeline(Context* ctx) {

  vkDestroyPipeline(ctx->device, ctx->pipeline, NULL);
  vkDestroyPipelineLayout(ctx->device, ctx->pipeline_layout, NULL);
  vkDestroyShaderModule(ctx->device, ctx->compute_module, NULL);
  free(ctx->compute_shader_code);
}