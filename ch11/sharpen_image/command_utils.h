#ifndef COMMAND_UTILS_H_
#define COMMAND_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"

VkCommandBufferBeginInfo create_command_buffers(Context *ctx, 
  VkCommandBuffer* command_buffer, VkCommandPool *command_pool,
  uint32_t count, uint32_t type, VkCommandBufferUsageFlags flags);

void destroy_command_buffers(Context* ctx, uint32_t num_command_buffers, 
  VkCommandPool* command_pool, VkCommandBuffer* buffers);

#endif /* COMMAND_UTILS_H_ */