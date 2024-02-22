#include "descriptor_utils.h"

/* Create descriptor set layout */
void create_descriptor_set_layout(Context* ctx, uint32_t num_bindings,
  VkShaderStageFlagBits stages[], VkDescriptorType types[], 
  VkSampler* samplers, VkDescriptorSetLayout* layout) {

  /* Allocate memory for bindings */
  VkDescriptorSetLayoutBinding* bindings = (VkDescriptorSetLayoutBinding*)
    malloc(num_bindings*sizeof(VkDescriptorSetLayoutBinding));
  
  /* Initialize bindings */
  for(uint32_t i=0; i<num_bindings; i++) {

    /* Set binding values */
    bindings[i].binding = i;
    bindings[i].descriptorType = types[i];
    bindings[i].descriptorCount = 1;
    bindings[i].stageFlags = stages[i];
    bindings[i].pImmutableSamplers = NULL;
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
    &descriptor_set_info, NULL, layout));

  /* Free memory for bindings */    
  free(bindings);
}

/* Create descriptor set */
void create_descriptor_set(Context* ctx, uint32_t num_pool_sizes, 
  VkDescriptorPoolSize pool_sizes[], VkDescriptorSetLayout* layout,
  VkDescriptorPool* pool, VkDescriptorSet* set) {

  /* Create descriptor pool info */
  VkDescriptorPoolCreateInfo descriptor_pool_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
    .maxSets = 1,
    .poolSizeCount = num_pool_sizes,
    .pPoolSizes = pool_sizes
  };

  /* Create descriptor pool */
  ck(vkCreateDescriptorPool(ctx->device, &descriptor_pool_info,
    NULL, pool));

  /* Create descriptor set allocation info */
  VkDescriptorSetAllocateInfo allocation_info = {
    .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
    .pNext = NULL,
    .descriptorPool = *pool,
    .descriptorSetCount = 1,
    .pSetLayouts = layout
  };

  /* Allocate memory for descriptor set */
  ck(vkAllocateDescriptorSets(ctx->device, &allocation_info, set));
}

/* Set the content of a buffer descriptor */
void init_buffer_descriptor(Context* ctx, VkBuffer* buffer,
  uint32_t binding, VkDeviceSize offset, VkDeviceSize size,
  VkDescriptorType type, VkDescriptorSet* set) {

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
    .dstSet = *set,
    .dstBinding = binding,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = type,
    .pImageInfo = NULL,
    .pBufferInfo = &buffer_info,
    .pTexelBufferView = NULL
  };

  /* Associate data with the descriptor set */
  vkUpdateDescriptorSets(ctx->device, 1, &write_descriptor_set, 0, NULL);
}

/* Set the content of a uniform descriptor */
void init_image_descriptor(Context* ctx, VkImage* image,
  uint32_t binding, VkFormat format, VkSampler* sampler,
  VkDescriptorType type, VkImageView* image_view, VkDescriptorSet* set) {

  /* Create buffer info */
  VkDescriptorImageInfo image_info = {
    .sampler = (sampler == NULL) ? NULL : *sampler,
    .imageView = *image_view,
    .imageLayout = VK_IMAGE_LAYOUT_GENERAL
  };

  /* Create info for associating data with descriptor set */
  VkWriteDescriptorSet write_descriptor_set = {
    .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
    .pNext = NULL,
    .dstSet = *set,
    .dstBinding = binding,
    .dstArrayElement = 0,
    .descriptorCount = 1,
    .descriptorType = type,
    .pImageInfo = &image_info,
    .pBufferInfo = NULL,
    .pTexelBufferView = NULL
  };

  /* Associate data with the descriptor set */
  vkUpdateDescriptorSets(ctx->device, 1, &write_descriptor_set, 0, NULL);
}

void destroy_descriptor_set(Context *ctx, VkDescriptorSetLayout* layout,
  VkDescriptorPool* pool, VkDescriptorSet* set) {

  vkFreeDescriptorSets(ctx->device, *pool, 1, set);
  vkDestroyDescriptorPool(ctx->device, *pool, NULL);
  vkDestroyDescriptorSetLayout(ctx->device, *layout, NULL);
}