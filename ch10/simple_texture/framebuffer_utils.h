#ifndef FRAMEBUFFER_UTILS_H_
#define FRAMEBUFFER_UTILS_H_

#include <vulkan/vulkan.h>
#include "device_utils.h"
#include "error_utils.h"

extern void create_image_view(Context* ctx, VkFormat format, 
  VkImageAspectFlags flags, VkImage* image, 
  VkImageView *image_view);

void create_framebuffers(Context* ctx);

void destroy_framebuffers(Context* ctx);

#endif /* FRAMEBUFFER_UTILS_H_ */