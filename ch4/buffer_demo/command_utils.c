#include "command_utils.h"

/* Create command buffers */
VkCommandBufferBeginInfo create_command_buffer(Context* ctx) {
  
  /* Create the command pool */
  const VkCommandPoolCreateInfo command_pool_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
    .queueFamilyIndex = ctx->family_indices[0]
  };
  vkCreateCommandPool(ctx->device, &command_pool_info, NULL, &ctx->command_pool);

  /* Allocate the command buffers */
  const VkCommandBufferAllocateInfo command_buffer_allocate_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = ctx->command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = 1
  };
  vkAllocateCommandBuffers(ctx->device, &command_buffer_allocate_info, &ctx->command_buffer);
  
  /* Create structure for recording commands */
  VkCommandBufferBeginInfo begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    .pInheritanceInfo = NULL
  };

  return begin_info;  
}

void destroy_command_buffer(Context* ctx) {
  
  vkFreeCommandBuffers(ctx->device, ctx->command_pool, 1, &ctx->command_buffer);
  vkDestroyCommandPool(ctx->device, ctx->command_pool, NULL);
}