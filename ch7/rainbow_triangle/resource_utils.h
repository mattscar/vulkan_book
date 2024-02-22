#ifndef RESOURCE_UTILS_H_
#define RESOURCE_UTILS_H_

#include "context.h"
#include "error_utils.h"

void create_buffer(Context* ctx, VkBufferUsageFlags usage, uint32_t size, 
  void* data, VkDeviceMemory* device_mem, VkBuffer* buffer);

void create_image_view(Context* ctx, VkFormat format, VkImageAspectFlags flags, 
  VkImage* image, VkImageView *image_view);
  
void destroy_buffer(Context* ctx, VkDeviceMemory* device_mem, VkBuffer* buffer);

#endif /* RESOURCE_UTILS_H_ */