#include "sync_utils.h"

void set_image_barriers(Context* ctx, VkImageMemoryBarrier* clear_barrier, VkImageMemoryBarrier* present_barrier, uint32_t i) {
  
  /* Define range of image affected by barriers */
  VkImageSubresourceRange subresource_range = {
    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel = 0,
    .levelCount = 1,
    .baseArrayLayer = 0,
    .layerCount = 1
  };  
  
  /* Configure the image barrier */
  clear_barrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  clear_barrier->pNext = NULL;
  clear_barrier->srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  clear_barrier->dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  clear_barrier->image = ctx->images[i];
  clear_barrier->oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  clear_barrier->newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  clear_barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  clear_barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  clear_barrier->subresourceRange = subresource_range;

  /* Configure the clear barrier */
  present_barrier->sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  present_barrier->pNext = NULL;
  present_barrier->srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  present_barrier->dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  present_barrier->oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  present_barrier->newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  present_barrier->srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  present_barrier->dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  present_barrier->image = ctx->images[i];
  present_barrier->subresourceRange = subresource_range;
}

void create_semaphores(Context *ctx) {
  
  /* Create semaphores for synchronizing commands */
  VkSemaphoreCreateInfo semaphore_info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  ck(vkCreateSemaphore(ctx->device, &semaphore_info, NULL, &ctx->acquire_complete_semaphore));
  ck(vkCreateSemaphore(ctx->device, &semaphore_info, NULL, &ctx->command_complete_semaphore));
}

void destroy_semaphores(Context *ctx) {
  vkDestroySemaphore(ctx->device, ctx->acquire_complete_semaphore, NULL);
  vkDestroySemaphore(ctx->device, ctx->command_complete_semaphore, NULL);  
}