#include "command_utils.h"

VkCommandBufferBeginInfo create_command_buffers(Context *ctx, 
  VkCommandBuffer* command_buffers, VkCommandPool *command_pool,
  uint32_t count, uint32_t type, VkCommandBufferUsageFlags flags) {

  /* Create command pool */
  VkCommandPoolCreateInfo command_pool_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = ctx->family_indices[type]
  };
  ck(vkCreateCommandPool(ctx->device, &command_pool_info, NULL, command_pool));

  /* Allocate memory for command buffers */
  VkCommandBufferAllocateInfo allocate_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = *command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = count
  };
  ck(vkAllocateCommandBuffers(ctx->device, &allocate_info, command_buffers));

  /* Create structure for recording commands */
  VkCommandBufferBeginInfo begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = flags,
    .pInheritanceInfo = NULL
  };

  return begin_info;
}

void destroy_command_buffers(Context* ctx, uint32_t num_command_buffers, 
  VkCommandPool* command_pool, VkCommandBuffer* buffers) {

  vkFreeCommandBuffers(ctx->device, *command_pool, num_command_buffers, buffers);
  if(num_command_buffers > 1) {
    free(ctx->command_buffers);
  }
  vkDestroyCommandPool(ctx->device, *command_pool, NULL);
}