#ifndef FRAMEBUFFER_UTILS_H_
#define FRAMEBUFFER_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "ErrorUtils.hpp"

extern void createImageView(Context& ctx, vk::Format format,
  vk::ImageAspectFlags flags, vk::Image& image,
  vk::ImageView& imageView);

void createFramebuffers(Context& ctx);

void destroyFramebuffers(Context& ctx);

#endif // FRAMEBUFFER_UTILS_H_ 