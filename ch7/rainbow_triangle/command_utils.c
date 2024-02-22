#include "command_utils.h"

VkCommandBufferBeginInfo create_command_buffers(Context *ctx) {

  /* Create command pool */
  VkCommandPoolCreateInfo command_pool_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueFamilyIndex = ctx->family_indices[0]
  };
  ck(vkCreateCommandPool(ctx->device, &command_pool_info, NULL, &ctx->command_pool));

  /* Allocate memory for command buffers */
  VkCommandBufferAllocateInfo allocate_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
    .pNext = NULL,
    .commandPool = ctx->command_pool,
    .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
    .commandBufferCount = ctx->num_images
  };
  ctx->command_buffers = (VkCommandBuffer*)malloc(ctx->num_images * sizeof(VkCommandBuffer));
  ck(vkAllocateCommandBuffers(ctx->device, &allocate_info, ctx->command_buffers));  
  
  /* Create structure for recording commands */
  VkCommandBufferBeginInfo begin_info = {
    .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
    .pNext = NULL,
    .flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT,
    .pInheritanceInfo = NULL
  };

  return begin_info;
}

void destroy_command_buffers(Context* ctx) {
  
  vkFreeCommandBuffers(ctx->device, ctx->command_pool, ctx->num_images, ctx->command_buffers);  
  free(ctx->command_buffers);    
  vkDestroyCommandPool(ctx->device, ctx->command_pool, NULL);
}