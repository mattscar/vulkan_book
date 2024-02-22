#ifndef COMMAND_UTILS_H_
#define COMMAND_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"

namespace command_utils {

vk::CommandBufferBeginInfo createCommandBuffers(Context& ctx,
  std::vector<vk::CommandBuffer>& commandBuffers, vk::CommandPool& commandPool,
  uint32_t count, uint32_t type, vk::CommandBufferUsageFlags flags, 
  vk::CommandBufferLevel level = vk::CommandBufferLevel::ePrimary);

void destroyCommandBuffers(vk::Device device,
  std::vector<vk::CommandBuffer> commandBuffers,
  vk::CommandPool commandPool);

}

#endif // COMMAND_UTILS_H_ 