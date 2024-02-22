#ifndef COMMAND_UTILS_H_
#define COMMAND_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"

vk::CommandBufferBeginInfo createCommandBuffers(Context& ctx, 
  std::vector<vk::CommandBuffer>& commandBuffers, vk::CommandPool& commandPool,
  uint32_t count, uint32_t type, vk::CommandBufferUsageFlags flags);

void destroyCommandBuffers(Context& ctx);

#endif // COMMAND_UTILS_H_ 