#include "compute_utils.h"

/* Create and configure a graphical pipeline */
void create_compute_pipeline(Context* ctx) {

  /* Read shader data */
  uint32_t compute_shader_length = 
    read_binary_file("shaders/comp.spv", &ctx->compute_shader_code);

  /* Pipeline layout data */
  VkPipelineLayoutCreateInfo pipeline_layout_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .setLayoutCount = 1,
    .pSetLayouts = &ctx->compute_desc_layout,
    .pushConstantRangeCount = 0,
    .pPushConstantRanges = NULL
  };

  printf("0: %u\n", compute_shader_length);  
  
  /* Create the pipeline layout */
  ck(vkCreatePipelineLayout(ctx->device, &pipeline_layout_info, NULL, &ctx->compute_pipeline_layout));

  printf("1\n");  

  /* Create compute shader module */
  VkShaderModuleCreateInfo compute_shader_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = compute_shader_length,
    .pCode = (uint32_t*)(ctx->compute_shader_code)
  };
  ck(vkCreateShaderModule(ctx->device, &compute_shader_create_info, NULL, &ctx->compute_module));

  printf("2\n");    
  
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
  
  printf("3\n");      
  
  /* Pipeline configuration info */
  VkComputePipelineCreateInfo pipeline_create_info = {
    .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stage = shader_stage_info,
    .layout = ctx->compute_pipeline_layout,
    .basePipelineHandle = NULL,
    .basePipelineIndex = -1
  };
  
  printf("4\n");  
  
  /* Create the compute pipeline */
  ck(vkCreateComputePipelines(ctx->device, NULL, 1, &pipeline_create_info, NULL, &ctx->compute_pipeline));
  
  printf("5\n");
}

/* Deallocate resources */
void destroy_compute_pipeline(Context* ctx) {

  vkDestroyPipeline(ctx->device, ctx->compute_pipeline, NULL);
  vkDestroyPipelineLayout(ctx->device, ctx->compute_pipeline_layout, NULL);
  vkDestroyShaderModule(ctx->device, ctx->compute_module, NULL);
  free(ctx->compute_shader_code);
}