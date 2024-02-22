#ifndef FRAMEBUFFER_UTILS_H_
#define FRAMEBUFFER_UTILS_H_

#include <vulkan/vulkan.h>
#include "device_utils.h"
#include "error_utils.h"

extern void create_image(Context* ctx, VkImageUsageFlags usage, VkFormat format, 
  VkMemoryPropertyFlags type, uint32_t size, void* data, 
  VkDeviceMemory* device_mem, VkImage* image);
  
extern void destroy_image(Context* ctx, VkImage* image, VkDeviceMemory* memory);

void create_framebuffers(Context* ctx);

void destroy_framebuffers(Context* ctx);

#endif /* FRAMEBUFFER_UTILS_H_ */