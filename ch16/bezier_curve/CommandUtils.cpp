#include "CommandUtils.hpp"

vk::CommandBufferBeginInfo createCommandBuffers(Context& ctx, 
  std::vector<vk::CommandBuffer>& commandBuffers, vk::CommandPool& commandPool,
  uint32_t count, uint32_t type, vk::CommandBufferUsageFlags flags) {
  
  // Create command pool 
  vk::CommandPoolCreateInfo commandPoolInfo = vk::CommandPoolCreateInfo()
    .setQueueFamilyIndex(ctx.familyIndices[type]);
  commandPool = ctx.device.createCommandPool(commandPoolInfo);
  
  // Allocate memory for command buffers 
  vk::CommandBufferAllocateInfo allocateInfo = vk::CommandBufferAllocateInfo()
    .setCommandPool(commandPool)
    .setLevel(vk::CommandBufferLevel::ePrimary)
    .setCommandBufferCount(count);
  commandBuffers = ctx.device.allocateCommandBuffers(allocateInfo);    
  
  // Create structure for recording commands 
  vk::CommandBufferBeginInfo beginInfo = vk::CommandBufferBeginInfo()
    .setFlags(flags);

  return beginInfo;
}

void destroyCommandBuffers(Context& ctx) {

  ctx.device.freeCommandBuffers(ctx.commandPool, ctx.commandBuffers);
  ctx.device.destroyCommandPool(ctx.commandPool, NULL);
}