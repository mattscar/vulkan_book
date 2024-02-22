#ifndef RESOURCE_UTILS_H_
#define RESOURCE_UTILS_H_

#include "context.h"
#include "error_utils.h"

void create_buffer(Context* ctx, VkBufferUsageFlags usage, VkMemoryPropertyFlags type, uint32_t size, void* data, VkDeviceMemory* device_mem, VkBuffer* buffer);
void destroy_buffer(Context* ctx, VkBuffer* buffer, VkDeviceMemory* device_mem);

#endif /* RESOURCE_UTILS_H_ */