#ifndef DESCRIPTOR_UTILS_H_
#define DESCRIPTOR_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include "device_utils.h"

/* Create descriptor set layout */
void create_descriptor_set_layout(Context* ctx, uint32_t num_bindings,
  VkShaderStageFlagBits stages[], VkDescriptorType types[], 
  VkSampler* samplers, VkDescriptorSetLayout* layout);

/* Create descriptor set */
void create_descriptor_set(Context* ctx, uint32_t num_pool_sizes, 
  VkDescriptorPoolSize pool_sizes[], VkDescriptorSetLayout* layout,
  VkDescriptorPool* pool, VkDescriptorSet* set);
  
/* Associate buffer data with a descriptor set */
void init_buffer_descriptor(Context* ctx, VkBuffer* buffer,
  uint32_t binding, VkDeviceSize offset, VkDeviceSize size,
  VkDescriptorType type, VkDescriptorSet* set);

/* Assign image data to a descriptor set */
void init_image_descriptor(Context* ctx, VkImage* image,
  uint32_t binding, VkFormat format, VkSampler* sampler,
  VkDescriptorType type, VkImageView* image_view, VkDescriptorSet* set);  
  
/* Destroy descriptor sets */
void destroy_descriptor_set(Context *ctx, VkDescriptorSetLayout* layout,
  VkDescriptorPool* pool, VkDescriptorSet* set);

#endif /* DESCRIPTOR_UTILS_H_ */