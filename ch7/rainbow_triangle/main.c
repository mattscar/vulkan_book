#include "command_utils.h"
#include "device_utils.h"
#include "framebuffer_utils.h"
#include "pipeline_utils.h"
#include "render_pass_utils.h"
#include "resource_utils.h"
#include "surface_utils.h"
#include "swapchain_utils.h"
#include "sync_utils.h"

#define NUM_VERTICES 3

/* Structure for vertex data */
typedef struct Vertex {
  float position[2];
  float color[3];
} Vertex;

void record_commands(Context* ctx);

/* Submit commands for execution and the image for presentation */
void draw(Context* ctx) {
  uint32_t index;

  /* Acquire the next image */
  VkResult result = vkAcquireNextImageKHR(ctx->device, ctx->swapchain,
    UINT64_MAX, ctx->acquire_complete_semaphore, NULL, &index);
  if(result == VK_ERROR_OUT_OF_DATE_KHR) {
    return;
  }

  /* Submit the command buffer to the queue */
  ctx->submit_info.pCommandBuffers = &ctx->command_buffers[index];
  ck(vkQueueSubmit(ctx->queues[0], 1, &ctx->submit_info, NULL));

  /* Present the image */
  ctx->present_info.pImageIndices = &index;
  ck(vkQueuePresentKHR(ctx->queues[0], &ctx->present_info));
}

int main() {
  Context ctx;
  ctx.swapchain = NULL;
  ctx.command_buffers = NULL;
  ctx.images = NULL;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;
  VkClearValue clear = {{1.0, 1.0, 1.0, 0.0}};

  /* Create vertex buffer */
  Vertex vertex_data[NUM_VERTICES] = {
    {{ 0.0, -0.5}, {1.0, 0.0, 0.0}},
    {{-0.5,  0.5}, {0.0, 1.0, 0.0}},
    {{ 0.5,  0.5}, {0.0, 0.0, 1.0}}
  };

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] = {VK_QUEUE_GRAPHICS_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Rainbow Triangle"); 
  create_surface(&ctx);
  create_sync(&ctx);
  create_render_pass(&ctx, 1, &clear);
  create_pipeline(&ctx);  

  /* Create buffer for vertex data */
  create_buffer(&ctx, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
    NUM_VERTICES * sizeof(Vertex), vertex_data,
    &ctx.vertex_buffer_memory, &ctx.vertex_buffer);
    
  /* Initialize the application */
  record_commands(&ctx);
  return 0;
}

void record_commands(Context* ctx) {

  uint32_t i;
  VkDeviceSize offset = 0;
  VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkCommandBufferBeginInfo command_begin_info;

  /* Create structures */
  create_swapchain(ctx);
  if(!ctx->command_buffers) {
    command_begin_info = create_command_buffers(ctx);
  }
  create_framebuffers(ctx);
  create_draw(ctx, &stage);

  /* Record commands to the command buffer */
  for(i=0; i<ctx->num_images; i++) {

    /* Record commands to the command buffer */
    ck(vkBeginCommandBuffer(ctx->command_buffers[i],
      &command_begin_info));

    ctx->draw_barrier.image = ctx->images[i];
    vkCmdPipelineBarrier(ctx->command_buffers[i],
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      0, 0, NULL, 0, NULL, 1, &ctx->draw_barrier);

    /* Bind the vertex buffer to the command buffer */
    vkCmdBindVertexBuffers(ctx->command_buffers[i], 0, 1, &ctx->vertex_buffer, &offset);

    /* Record commands to the render pass */
    ctx->render_pass_begin_info.framebuffer = ctx->framebuffers[i];
    vkCmdBeginRenderPass(ctx->command_buffers[i],
      &ctx->render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    /* Bind the graphics pipeline to the command buffer */
    vkCmdBindPipeline(ctx->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, ctx->pipeline);

    /* Perform the draw operation */
    vkCmdDraw(ctx->command_buffers[i], NUM_VERTICES, 1, 0, 0);

    /* Finish recording commands to the render pass */
    vkCmdEndRenderPass(ctx->command_buffers[i]);

    ctx->present_barrier.image = ctx->images[i];
    vkCmdPipelineBarrier(ctx->command_buffers[i],
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
      VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
      0, 0, NULL, 0, NULL, 1, &ctx->present_barrier);

    /* Finish recording commands to the command buffer */
    ck(vkEndCommandBuffer(ctx->command_buffers[i]));
  }

  /* Display the window */
  display_surface(ctx);
}

/* Resize window */
void resize_window(Context* ctx) {

  /* Destroy framebuffers */
  destroy_framebuffers(ctx);

  /* Reset command buffer and record commands */
  for(uint32_t i=0; i<ctx->num_images; i++) {
    vkResetCommandBuffer(ctx->command_buffers[i], 0);
  }
  record_commands(ctx);
}

/* Deallocate structures */
void destroy(Context* ctx) {

  destroy_buffer(ctx, &ctx->vertex_buffer_memory, &ctx->vertex_buffer);
  destroy_pipeline(ctx);
  destroy_framebuffers(ctx);
  destroy_render_pass(ctx);
  destroy_sync(ctx);
  destroy_swapchain(ctx);
  destroy_command_buffers(ctx);
  destroy_surface(ctx);
  destroy_device(ctx);
}

