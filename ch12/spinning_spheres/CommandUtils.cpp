#include "CommandUtils.hpp"

namespace command_utils {

vk::CommandBufferBeginInfo createCommandBuffers(Context& ctx,
  std::vector<vk::CommandBuffer>& commandBuffers, vk::CommandPool& commandPool,
  uint32_t count, uint32_t type, vk::CommandBufferUsageFlags flags, 
  vk::CommandBufferLevel level) {

  // Create command pool if it doesn't already exist
  if(!commandPool) {
    vk::CommandPoolCreateInfo commandPoolInfo = vk::CommandPoolCreateInfo()
      .setQueueFamilyIndex(ctx.familyIndices[type]);
    auto poolRes = ctx.device.createCommandPool(commandPoolInfo);
    ck(poolRes.result);
    commandPool = poolRes.value;
  }

  // Allocate memory for command buffers
  vk::CommandBufferAllocateInfo allocateInfo = vk::CommandBufferAllocateInfo()
    .setCommandPool(commandPool)
//    .setLevel(primary ? vk::CommandBufferLevel::ePrimary : vk::CommandBufferLevel::eSecondary)
    .setLevel(level)    
    .setCommandBufferCount(count);
  auto commandRes = ctx.device.allocateCommandBuffers(allocateInfo);
  ck(commandRes.result);
  commandBuffers = commandRes.value;

  // Create structure for recording commands
  vk::CommandBufferBeginInfo beginInfo = vk::CommandBufferBeginInfo()
    .setFlags(flags);

  return beginInfo;
}

void destroyCommandBuffers(vk::Device device,
  std::vector<vk::CommandBuffer> commandBuffers,
  vk::CommandPool commandPool) {

  device.freeCommandBuffers(commandPool, commandBuffers);
  device.destroyCommandPool(commandPool, NULL);
}

}