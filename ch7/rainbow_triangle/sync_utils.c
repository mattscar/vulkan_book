#include "sync_utils.h"

void create_sync(Context *ctx) {
  
  /* Create semaphores for synchronizing commands */
  VkSemaphoreCreateInfo semaphore_info = {
    .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0
  };
  ck(vkCreateSemaphore(ctx->device, &semaphore_info, NULL, &ctx->acquire_complete_semaphore));
  ck(vkCreateSemaphore(ctx->device, &semaphore_info, NULL, &ctx->command_complete_semaphore));
  
  /* Define range of image affected by barriers */
  VkImageSubresourceRange subresource_range = {
    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel = 0,
    .levelCount = 1,
    .baseArrayLayer = 0,
    .layerCount = 1
  };  

  /* Configure the barrier to set image layout before drawing */
  ctx->draw_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  ctx->draw_barrier.pNext = NULL;
  ctx->draw_barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  ctx->draw_barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  ctx->draw_barrier.oldLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
  ctx->draw_barrier.newLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  ctx->draw_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  ctx->draw_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  ctx->draw_barrier.subresourceRange = subresource_range;

  /* Configure the barrier to set image layout before presentation */
  ctx->present_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  ctx->present_barrier.pNext = NULL;
  ctx->present_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
  ctx->present_barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
  ctx->present_barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  ctx->present_barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  ctx->present_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  ctx->present_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  ctx->present_barrier.subresourceRange = subresource_range;  
}

void create_draw(Context *ctx, VkPipelineStageFlags *stage) {
  
  /* Configure the info for submitting commands to the queue */
  ctx->submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  ctx->submit_info.pNext = NULL;
  ctx->submit_info.waitSemaphoreCount = 1;
  ctx->submit_info.pWaitSemaphores = &ctx->acquire_complete_semaphore;
  ctx->submit_info.pWaitDstStageMask = stage;
  ctx->submit_info.commandBufferCount = 1;
  ctx->submit_info.signalSemaphoreCount = 1;
  ctx->submit_info.pSignalSemaphores = &ctx->command_complete_semaphore;
  
  /* Configure the info for presenting images */  
  ctx->present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  ctx->present_info.pNext = NULL;
  ctx->present_info.waitSemaphoreCount = 1;
  ctx->present_info.pWaitSemaphores = &ctx->command_complete_semaphore;
  ctx->present_info.swapchainCount = 1;
  ctx->present_info.pSwapchains = &ctx->swapchain;
  ctx->present_info.pResults = NULL;
}

void destroy_sync(Context *ctx) {

  vkDestroySemaphore(ctx->device, ctx->acquire_complete_semaphore, NULL);
  vkDestroySemaphore(ctx->device, ctx->command_complete_semaphore, NULL);  
}