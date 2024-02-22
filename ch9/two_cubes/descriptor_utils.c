#include "descriptor_utils.h"

void create_descriptor_set(Context* ctx, uint32_t num_bindings,
  VkDescriptorPoolSize pool_sizes[], VkShaderStageFlagBits stages[],
  VkDescriptorSetLayoutBinding bindings[], VkSampler* samplers) {

  /* Initialize bindings */
  for(uint32_t i=0; i<num_bindings; i++) {

    /* Set binding values */
    bindings[i].binding = i;
    bindings[i].descriptorType = pool_sizes[0].type;
    bindings[i].descriptorCount = 1;
    bindings[i].stageFlags = stages[i];

    /* Identify the samplers */
    if(stages[i] == VK_SHADER_STAGE_FRAGMENT_BIT) {
      bindings[i].pImmutableSamplers = NULL;
    } else {
      bindings[i].pImmutableSamplers = samplers;
    }
  }

  /* Create descriptor set layout info */
  VkDescriptorSetLayoutCreateInfo descriptor_set_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .bindingCount = num_bindings,
    .pBindings = bindings
  };

  /* Create descriptor set layout */
  ck(vkCreateDescriptorSetLayout(ctx->device,
    &descriptor_set_info, NULL, &ctx->descriptor_set_layout));

  /* Create descriptor pool info */
  VkDescriptorPoolCreateInfo descriptor_pool_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    .maxSets = 1,
    .poolSizeCount = 1,
    .pPoolSizes = pool_sizes
  };

  /* Create descriptor pool */
  ck(vkCreateDescriptorPool(ctx->device, &descriptor_pool_info,
    NULL, &ctx->descriptor_pool));

  /* Create descriptor set allocation info */
  VkDescriptorSetAllocateInfo allocation_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = NULL,
    .descriptorPool = ctx->descriptor_pool,
    .descriptorSetCount = 1,
    .pSetLayouts = &ctx->descriptor_set_layout
  };

  /* Allocate memory for descriptor set */
  ck(vkAllocateDescriptorSets(ctx->device, &allocation_info,
    &ctx->descriptor_set));
}

/* Set the content of the descriptor set */
void init_uniform_descriptor(Context* ctx, VkBuffer* buffer,
  uint32_t binding, VkDeviceSize offset, VkDeviceSize size) {

  /* Create buffer info */
  VkDescriptorBufferInfo buffer_info = {
    .buffer = *buffer,
    .offset = offset,
    .range = size
  };

  /* Create info for associating data with descriptor set */
  VkWriteDescriptorSet write_descriptor_set = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = NULL,
    .dstSet = ctx->descriptor_set,
    .dstBinding = binding,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
    .pImageInfo = NULL,
    .pBufferInfo = &buffer_info,
    .pTexelBufferView = NULL
  };

  /* Associate data with the descriptor set */
  vkUpdateDescriptorSets(ctx->device, 1, &write_descriptor_set, 0, NULL);
}

void destroy_descriptor_set(Context *ctx) {

  vkFreeDescriptorSets(ctx->device, ctx->descriptor_pool, 1, &ctx->descriptor_set);
  vkDestroyDescriptorPool(ctx->device, ctx->descriptor_pool, NULL);
  vkDestroyDescriptorSetLayout(ctx->device, ctx->descriptor_set_layout, NULL);
}