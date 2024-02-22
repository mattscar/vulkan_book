#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"
#include "command_utils.h"
#include "descriptor_utils.h"
#include "pipeline_utils.h"
#include "render_pass_utils.h"
#include "framebuffer_utils.h"
#include "resource_utils.h"
#include "surface_utils.h"
#include "swapchain_utils.h"
#include "sync_utils.h"

#define NUM_VERTICES 4
#define NUM_INSTANCES 4
#define NUM_DESCRIPTOR_BINDINGS 1

/* Structure for vertex data */
typedef struct Vertex_t {
  float position[2];
  float color[3];
} Vertex;

/* Structure for all draw data */
typedef struct DrawData_t {
  Vertex vertex_data[NUM_VERTICES];
  uint16_t index_data[NUM_VERTICES];
  VkDrawIndexedIndirectCommand indirect_data;
  float uniform_data[3];
} DrawData;

void record_commands(Context* ctx);

/* Submit the command buffer and present the processed image */
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
  ck(vkQueueSubmit(ctx->queues[GRAPHICS_QTYPE], 1, &ctx->submit_info, NULL));

  /* Present the image */
  ctx->present_info.pImageIndices = &index;
  ck(vkQueuePresentKHR(ctx->queues[GRAPHICS_QTYPE], &ctx->present_info));
}

int main() {
  Context ctx;
  VkDescriptorSetLayoutBinding set_bindings[NUM_DESCRIPTOR_BINDINGS];
  ctx.swapchain = NULL;
  ctx.command_buffers = NULL;
  ctx.images = NULL;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;
  VkClearValue clear = {{0.25, 0.25, 0.25, 0.0}};

  /* Initialize draw data */
  DrawData draw_data = {
    .vertex_data = { {{-0.25, -0.25}, {1.0, 1.0, 1.0}},
                     {{ 0.25, -0.25}, {1.0, 1.0, 1.0}},
                     {{ 0.25,  0.25}, {1.0, 1.0, 1.0}},
                     {{-0.25,  0.25}, {1.0, 1.0, 1.0}} },
    .index_data = {1, 0, 2, 3},
    .indirect_data = (VkDrawIndexedIndirectCommand) {
      .indexCount = NUM_VERTICES,
      .instanceCount = NUM_INSTANCES,
      .firstIndex = 0,
      .vertexOffset = 0,
      .firstInstance = 0
    },
    .uniform_data = {0.3, 0.3, 0.3}
  };

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] = 
    {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Four Squares");   
  create_surface(&ctx);
  create_sync(&ctx);
  create_render_pass(&ctx, 1, &clear);
  

  /* Create device buffer */
  create_buffer(&ctx,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
    VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(draw_data),
    NULL, &ctx.draw_buffer_memory, &ctx.draw_buffer);

  /* Transfer data from staging buffer to device buffer */
  transfer_to_device_buffer(&ctx, &draw_data, sizeof(draw_data), &ctx.draw_buffer);

  /* Configure uniform buffer */
  VkDescriptorPoolSize pool_sizes[1] = {
    {
      .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount = 1
    }
  };
  VkShaderStageFlagBits stages[NUM_DESCRIPTOR_BINDINGS] =
    {VK_SHADER_STAGE_VERTEX_BIT};
  create_descriptor_set(&ctx, NUM_DESCRIPTOR_BINDINGS, pool_sizes,
    stages, set_bindings, NULL);

  /* Write data to uniform descriptor */
  VkDeviceSize uniform_offset = sizeof(draw_data.vertex_data)
    + sizeof(draw_data.index_data) + sizeof(draw_data.indirect_data);
  init_uniform_descriptor(&ctx, &ctx.draw_buffer, 0, uniform_offset, 3*sizeof(float));

  /* Create pipeline */
  create_pipeline(&ctx);   
  
  /* Start recording commands */
  record_commands(&ctx);
  return 0;
}

void record_commands(Context* ctx) {

  uint32_t i;
  VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkCommandBufferBeginInfo command_begin_info;

  /* Initialize push constant data */
  float push_constant_data[8] = {-0.5, -0.5, -0.5, 0.5, 0.5, 0.5, 0.5, -0.5};

  /* Create structures */
  create_swapchain(ctx);
  
  if(!ctx->command_buffers) {
    ctx->command_buffers = 
      (VkCommandBuffer*)malloc(ctx->num_images * sizeof(VkCommandBuffer));
    command_begin_info = 
      create_command_buffers(ctx, ctx->command_buffers,
        &ctx->command_pool, ctx->num_images, GRAPHICS_QTYPE,
        VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);
  }
  create_framebuffers(ctx);  
  create_draw(ctx, &stage);

  /* Set buffer offsets */
  VkDeviceSize vertex_offset = 0;
  VkDeviceSize index_offset = NUM_VERTICES * sizeof(Vertex);
  VkDeviceSize indirect_offset = index_offset + NUM_VERTICES * sizeof(uint16_t);

  /* Record commands to the command buffer */
  for (i=0; i<ctx->num_images; i++) {

    /* Record commands to the command buffer */
    ck(vkBeginCommandBuffer(ctx->command_buffers[i],
      &command_begin_info));

    ctx->draw_barrier.image = ctx->images[i];
    vkCmdPipelineBarrier(ctx->command_buffers[i],
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      VK_PIPELINE_STAGE_TRANSFER_BIT,
      0, 0, NULL, 0, NULL, 1, &ctx->draw_barrier);

    /* Bind the vertex buffer to the command buffer */
    vkCmdBindVertexBuffers(ctx->command_buffers[i],
      0, 1, &ctx->draw_buffer, &vertex_offset);

    /* Bind the index buffer to the command buffer */
    vkCmdBindIndexBuffer(ctx->command_buffers[i],
      ctx->draw_buffer, index_offset, VK_INDEX_TYPE_UINT16);

    /* Bind the descriptor set */
    vkCmdBindDescriptorSets(ctx->command_buffers[i],
      VK_PIPELINE_BIND_POINT_GRAPHICS, ctx->pipeline_layout,
      0, 1, &ctx->descriptor_set, 0, NULL);

    /* Bind push constant data to the command buffer */
    vkCmdPushConstants(ctx->command_buffers[i],
      ctx->pipeline_layout, VK_SHADER_STAGE_VERTEX_BIT,
      0, sizeof(push_constant_data), push_constant_data);

    /* Record commands to the render pass */
    ctx->render_pass_begin_info.framebuffer = ctx->framebuffers[i];
    vkCmdBeginRenderPass(ctx->command_buffers[i],
      &ctx->render_pass_begin_info, VK_SUBPASS_CONTENTS_INLINE);

    /* Bind the graphics pipeline to the command buffer */
    vkCmdBindPipeline(ctx->command_buffers[i],
      VK_PIPELINE_BIND_POINT_GRAPHICS, ctx->pipeline);

    /* Perform the draw operation */
    vkCmdDrawIndexedIndirect(ctx->command_buffers[i],
      ctx->draw_buffer, indirect_offset, 1, 0);

    /* Finish recording commands to the render pass */
    vkCmdEndRenderPass(ctx->command_buffers[i]);

    ctx->present_barrier.image = ctx->images[i];
    vkCmdPipelineBarrier(ctx->command_buffers[i],
      VK_PIPELINE_STAGE_TRANSFER_BIT,
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

  destroy_buffer(ctx, &ctx->draw_buffer, &ctx->draw_buffer_memory);
  destroy_pipeline(ctx);
  destroy_descriptor_set(ctx);
  destroy_framebuffers(ctx);  
  destroy_render_pass(ctx);
  destroy_sync(ctx);
  destroy_command_buffers(ctx);
  destroy_swapchain(ctx);
  destroy_surface(ctx);
  destroy_device(ctx);
}

