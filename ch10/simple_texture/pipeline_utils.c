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

  uint32_t vertex_shader_length, fragment_shader_length;

  /* Read shader data */
  vertex_shader_length = read_binary_file("shaders/vert.spv", &ctx->vertex_shader_code);
  fragment_shader_length = read_binary_file("shaders/frag.spv", &ctx->fragment_shader_code);

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
  VkShaderModuleCreateInfo vertex_shader_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = vertex_shader_length,
    .pCode = (uint32_t*)(ctx->vertex_shader_code)
  };
  ck(vkCreateShaderModule(ctx->device, &vertex_shader_create_info, NULL, &ctx->vertex_module));

  /* Create fragment shader module */
  VkShaderModuleCreateInfo fragment_shader_create_info = {
    .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .codeSize = fragment_shader_length,
    .pCode = (uint32_t*)(ctx->fragment_shader_code)
  };
  ck(vkCreateShaderModule(ctx->device, &fragment_shader_create_info, NULL, &ctx->fragment_module));

  /* Shader configuration data */
  VkPipelineShaderStageCreateInfo shader_stage_infos[2] = {

    /* Vertex shader */
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = ctx->vertex_module,
      .pName = "main",
      .pSpecializationInfo = NULL
    },

    /* Fragment shader */
    {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = ctx->fragment_module,
      .pName = "main",
      .pSpecializationInfo = NULL
    }
  };

  VkVertexInputBindingDescription binding_info = {
    .binding = 0,
    .stride = 4 * sizeof(float),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
  };

  VkVertexInputAttributeDescription attribute_infos[2] = {
    /* Vertex coordinates */
    {
      .location = 0,
      .binding = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = 0
    },
    /* Texture coordinates */
    {
      .location = 1,
      .binding = 0,
      .format = VK_FORMAT_R32G32_SFLOAT,
      .offset = 2 * sizeof(float)
    }
  };

  VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .vertexBindingDescriptionCount = 1,
    .pVertexBindingDescriptions = &binding_info,
    .vertexAttributeDescriptionCount = 2,
    .pVertexAttributeDescriptions = attribute_infos
  };

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    .primitiveRestartEnable = VK_FALSE
  };

  /* Define the pipeline's viewport */
  VkViewport viewport = {
    .x = 1.0f * ctx->x,
    .y = 1.0f * ctx->y,
    .width = 1.0f * ctx->width,
    .height = 1.0f * ctx->height,
    .minDepth = 0.0f,
    .maxDepth = 1.0f
  };

  /* Define a scissor */
  VkRect2D scissor = {
    .offset = {
      .x = 0,
      .y = 0
    },
    .extent = {
      .width = ctx->width,
      .height = ctx->height
    }
  };

  VkPipelineViewportStateCreateInfo viewport_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .viewportCount = 1,
    .pViewports = &viewport,
    .scissorCount = 1,
    .pScissors = &scissor
  };

  VkPipelineRasterizationStateCreateInfo rasterization_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .depthClampEnable = VK_FALSE,
    .rasterizerDiscardEnable = VK_FALSE,
    .polygonMode = VK_POLYGON_MODE_FILL,
    .cullMode = VK_CULL_MODE_NONE,
    .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
    .depthBiasEnable = VK_FALSE,
    .depthBiasConstantFactor = 0.0f,
    .depthBiasClamp = 0.0f,
    .depthBiasSlopeFactor = 0.0f,
    .lineWidth = 3.0f
  };

  VkPipelineMultisampleStateCreateInfo multisample_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    .sampleShadingEnable = VK_FALSE,
    .minSampleShading = 1.0f,
    .pSampleMask = NULL,
    .alphaToCoverageEnable = VK_FALSE,
    .alphaToOneEnable = VK_FALSE
  };

  VkPipelineColorBlendAttachmentState color_blend_attachment_state = {
    .blendEnable = VK_FALSE,
    .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
    .colorBlendOp = VK_BLEND_OP_ADD,
    .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
    .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
    .alphaBlendOp = VK_BLEND_OP_ADD,
    .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
    VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
  };

  VkPipelineColorBlendStateCreateInfo color_blend_state_create_info = {
    .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .logicOpEnable = VK_FALSE,
    .logicOp = VK_LOGIC_OP_COPY,
    .attachmentCount = 1,
    .pAttachments = &color_blend_attachment_state,
    .blendConstants = { 0.0f, 0.0f, 0.0f, 0.0f }
  };

  /* Create the info that configures the graphic pipeline */
  VkGraphicsPipelineCreateInfo pipeline_create_info = {
    .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .stageCount = 2,
    .pStages = shader_stage_infos,
    .pVertexInputState = &vertex_input_state_create_info,
    .pInputAssemblyState = &input_assembly_state_create_info,
    .pTessellationState = NULL,
    .pViewportState = &viewport_state_create_info,
    .pRasterizationState = &rasterization_state_create_info,
    .pMultisampleState = &multisample_state_create_info,
    .pDepthStencilState = NULL,
    .pColorBlendState = &color_blend_state_create_info,
    .pDynamicState = NULL,
    .layout = ctx->pipeline_layout,
    .renderPass = ctx->render_pass,
    .subpass = 0,
    .basePipelineHandle = NULL,
    .basePipelineIndex = -1
  };

  /* Create the graphics pipeline */
  vkCreateGraphicsPipelines(ctx->device, NULL, 1, &pipeline_create_info, NULL, &ctx->pipeline);
}

/* Deallocate resources */
void destroy_pipeline(Context* ctx) {

  vkDestroyPipeline(ctx->device, ctx->pipeline, NULL);
  vkDestroyPipelineLayout(ctx->device, ctx->pipeline_layout, NULL);
  vkDestroyShaderModule(ctx->device, ctx->fragment_module, NULL);
  vkDestroyShaderModule(ctx->device, ctx->vertex_module, NULL);
  free(ctx->fragment_shader_code);
  free(ctx->vertex_shader_code);
}