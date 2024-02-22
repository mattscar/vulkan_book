#ifndef RESOURCE_UTILS_H_
#define RESOURCE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"
#include "error_utils.h"
#include "command_utils.h"

void create_buffer(Context* ctx, VkBufferUsageFlags usage, VkMemoryPropertyFlags type, 
  uint32_t size, void* data, VkDeviceMemory* device_mem, VkBuffer* buffer);

void transfer_to_device_buffer(Context* ctx, void* data,
  size_t data_size, VkBuffer* device_buffer);

void destroy_buffer(Context* ctx, VkBuffer* buffer, VkDeviceMemory* device_mem);

#endif /* RESOURCE_UTILS_H_ */