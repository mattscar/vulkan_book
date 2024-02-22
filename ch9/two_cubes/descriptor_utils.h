#ifndef DESCRIPTOR_UTILS_H_
#define DESCRIPTOR_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include "device_utils.h"

/* Create one or more descriptor sets */
void create_descriptor_set(Context* ctx, uint32_t num_bindings, VkDescriptorPoolSize pool_sizes[], 
  VkShaderStageFlagBits stages[], VkDescriptorSetLayoutBinding bindings[], VkSampler* samplers);
  
/* Assign uniform data to a descriptor set */
void init_uniform_descriptor(Context* ctx, VkBuffer* buffer, uint32_t binding,
  VkDeviceSize offset, VkDeviceSize size);

/* Destroy descriptor sets */
void destroy_descriptor_set(Context *ctx);

#endif /* DESCRIPTOR_UTILS_H_ */