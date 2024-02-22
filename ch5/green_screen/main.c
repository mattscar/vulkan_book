#include "context.h"
#include "device_utils.h"
#include "surface_utils.h"
#include "swapchain_utils.h"
#include "command_utils.h"
#include "sync_utils.h"

/* Submit the command buffer and image presentation */
void draw(Context* ctx) {
  uint32_t index;
  VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TRANSFER_BIT;

  /* Acquire the next image */
  VkResult result = vkAcquireNextImageKHR(ctx->device, ctx->swapchain, UINT64_MAX, 
    ctx->acquire_complete_semaphore, NULL, &index);
  if(result == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }
  
  /* Submit info data */
  VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &ctx->acquire_complete_semaphore,
    .pWaitDstStageMask = &stage,
    .commandBufferCount = 1,
    .pCommandBuffers = &ctx->command_buffers[index],
    .signalSemaphoreCount = 1,
    .pSignalSemaphores = &ctx->command_complete_semaphore
  };

  /* Submit the command buffer to the queue */
  ck(vkQueueSubmit(ctx->queues[0], 1, &submit_info, NULL));

  /* Presentation info data */
  VkPresentInfoKHR present_info = {
    .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    .pNext = NULL,
    .waitSemaphoreCount = 1,
    .pWaitSemaphores = &ctx->command_complete_semaphore,
    .swapchainCount = 1,
    .pSwapchains = &ctx->swapchain,
    .pImageIndices = &index,
    .pResults = NULL
  };

  /* Present the image */
  ck(vkQueuePresentKHR(ctx->queues[0], &present_info));
}

int main() {
  Context ctx;
  ctx.x = 100; ctx.y = 100;
  ctx.width = 200; ctx.height = 200;
  ctx.swapchain = NULL;  
  ctx.images = NULL;  
  
  VkCommandBufferBeginInfo command_begin_info;
  VkImageMemoryBarrier clear_barrier, present_barrier;
  VkClearColorValue green = { 0.1f, 1.0f, 0.1f, 0.0f };
  uint32_t i;

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] = {VK_QUEUE_GRAPHICS_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Green Screen");  
  create_surface(&ctx);
  create_swapchain(&ctx);
  command_begin_info = create_command_buffers(&ctx);
  create_semaphores(&ctx);

  /* Set range of image clear command */
  VkImageSubresourceRange range = {
    .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
    .baseMipLevel = 0,
    .levelCount = 1,
    .baseArrayLayer = 0,
    .layerCount = 1
  };

  /* Record commands to the command buffer */
  for (i = 0; i < ctx.num_images; i++) {

    /* Create image barriers */
    set_image_barriers(&ctx, &clear_barrier, 
      &present_barrier, i);

    /* Start recording commands */
    ck(vkBeginCommandBuffer(ctx.command_buffers[i], 
      &command_begin_info));

    vkCmdPipelineBarrier(ctx.command_buffers[i], 
      VK_PIPELINE_STAGE_TRANSFER_BIT, 
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      0, 0, NULL, 0, NULL, 1, &clear_barrier);

    /* Set the image's background color */
    vkCmdClearColorImage(ctx.command_buffers[i], ctx.images[i], 
      VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &green, 1, &range);

    vkCmdPipelineBarrier(ctx.command_buffers[i], 
      VK_PIPELINE_STAGE_TRANSFER_BIT, 
      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      0, 0, NULL, 0, NULL, 1, &present_barrier);

    /* Finish recording commands */
    ck(vkEndCommandBuffer(ctx.command_buffers[i]));
  }

  /* Display the window */
  display_surface(&ctx);
  return 0;
}

/* Deallocate structures */
void destroy(Context* ctx) {

  destroy_semaphores(ctx);
  destroy_swapchain(ctx);
  destroy_command_buffers(ctx);
  destroy_surface(ctx);
  destroy_device(ctx);
}

