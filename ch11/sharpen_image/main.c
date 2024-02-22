#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"
#include "command_utils.h"
#include "compute_utils.h"
#include "descriptor_utils.h"
#include "framebuffer_utils.h"
#include "gli_utils.h"
#include "pipeline_utils.h"
#include "render_pass_utils.h"
#include "resource_utils.h"
#include "surface_utils.h"
#include "swapchain_utils.h"
#include "sync_utils.h"

#include "gli/texture2d.hpp"
#include "gli/convert.hpp"
#include "gli/generate_mipmaps.hpp"
#include "gli/load.hpp"
#include "gli/save.hpp"
#include "glm/glm.hpp"

#define NUM_VERTICES 4

/* Structure for vertex data */
typedef struct Vertex_t {
  float position[2];
  float tex_coords[2];
} Vertex;

/* Structure for all draw data */
typedef struct DrawData_t {
  Vertex vertex_data[NUM_VERTICES];
  uint16_t index_data[NUM_VERTICES];
  VkDrawIndexedIndirectCommand indirect_data;
} DrawData;

void configure_descriptor_sets(Context* ctx, VkFormat format);
void record_commands(Context* ctx);
void execute_compute(Context* ctx);

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
  ctx.swapchain = NULL;
  ctx.command_buffers = NULL;
  ctx.images = NULL;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;
  VkClearValue clear = {{0.25, 0.25, 0.25, 0.0}};

  /* Initialize draw data */
  DrawData draw_data = {
    .vertex_data = { {{-0.5, -0.5}, { 0.0, 0.0}},
                     {{ 0.5, -0.5}, { 1.0, 0.0}},
                     {{ 0.5,  0.5}, { 1.0, 1.0}},
                     {{-0.5,  0.5}, { 0.0, 1.0}} },
    .index_data = {1, 0, 2, 3},
    .indirect_data = (VkDrawIndexedIndirectCommand) {
      .indexCount = NUM_VERTICES,
      .instanceCount = 1,
      .firstIndex = 0,
      .vertexOffset = 0,
      .firstInstance = 0
    }
  };

  /* Access texture image */
  gli::texture2d tex(gli::load("castle.ktx"));
  glm::ivec2 extent = tex.extent();
  uint32_t tex_width = (uint32_t)extent.x;
  uint32_t tex_height = (uint32_t)extent.y;
  uint32_t tex_size = (uint32_t)tex.size();
  VkFormat tex_format = convert_format(tex.format());

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] = {VK_QUEUE_GRAPHICS_BIT,
    VK_QUEUE_TRANSFER_BIT, VK_QUEUE_COMPUTE_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Sharpen Image");
  create_surface(&ctx);
  create_sync(&ctx);
  create_render_pass(&ctx, 1, &clear);

  /* Create buffer containing vertex/index data */
  create_buffer(&ctx,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
    VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(draw_data),
    NULL, &ctx.draw_buffer_memory, &ctx.draw_buffer);

  /* Transfer data from staging buffer to device buffer */
  transfer_to_device_buffer(&ctx, &draw_data, sizeof(draw_data),
    &ctx.draw_buffer);

  /* Create sampler, image, and image view */
  create_sampler(&ctx);
  create_image(&ctx, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
    tex_format, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, tex_size, NULL,
    tex_width, tex_height, &ctx.tex_image_memory, &ctx.tex_image);

  /* Transfer image to device using staging buffer */
  transfer_to_device_image(&ctx, tex.data(), tex_size, tex_width, tex_height,
    &ctx.tex_image);

  /* Configure descriptor sets */
  configure_descriptor_sets(&ctx, tex_format);

  /* Create pipelines */
  create_pipeline(&ctx);  
  create_compute_pipeline(&ctx);    

  /* Record and execute compute commands */
  execute_compute(&ctx);
  
  /* Record graphical commands */
  record_commands(&ctx);
  
  return 0;
}

void execute_compute(Context* ctx) {
  
  VkCommandBufferBeginInfo command_begin_info =
    create_command_buffers(ctx, &ctx->compute_command_buffer,
      &ctx->compute_command_pool, 1, COMPUTE_QTYPE,
      VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

  /* Record commands to the command buffer */
  ck(vkBeginCommandBuffer(ctx->compute_command_buffer,
    &command_begin_info));

  /* Bind the descriptor set */
  vkCmdBindDescriptorSets(ctx->compute_command_buffer,
    VK_PIPELINE_BIND_POINT_COMPUTE, ctx->compute_pipeline_layout,
    0, 1, &ctx->compute_desc_set, 0, NULL);

  /* Bind the compute pipeline to the command buffer */
  vkCmdBindPipeline(ctx->compute_command_buffer,
    VK_PIPELINE_BIND_POINT_COMPUTE, ctx->compute_pipeline);

  /* Record a command to perform computation */
  vkCmdDispatch(ctx->compute_command_buffer, 2, 2, 1);

  /* Finish recording commands to the command buffer */
  ck(vkEndCommandBuffer(ctx->compute_command_buffer));
  
  /* Submit transfer command to queue */
  VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = 0,
    .pWaitSemaphores = NULL,
    .pWaitDstStageMask = NULL,
    .commandBufferCount = 1,
    .pCommandBuffers = &ctx->compute_command_buffer,
    .signalSemaphoreCount = 0,
    .pSignalSemaphores = NULL
  };
  ck(vkQueueSubmit(ctx->queues[COMPUTE_QTYPE],
    1, &submit_info, NULL));

  /* Wait for transfer to complete */
  ck(vkQueueWaitIdle(ctx->queues[COMPUTE_QTYPE]));  
}

void record_commands(Context* ctx) {

  uint32_t i;
  VkPipelineStageFlags stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  VkCommandBufferBeginInfo command_begin_info;

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
      0, 1, &ctx->graphical_desc_set, 0, NULL);

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

/* Configure descriptor sets */
void configure_descriptor_sets(Context* ctx, VkFormat format) {

  /* Configure graphical descriptor set layout */
  VkShaderStageFlagBits stages[1] =
    {VK_SHADER_STAGE_FRAGMENT_BIT};
  VkDescriptorType types[1] =
    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER};
  create_descriptor_set_layout(ctx, 1,
    stages, types, NULL, &ctx->graphical_desc_layout);

  /* Configure graphical descriptor set */
  VkDescriptorPoolSize pool_sizes[1] =
    {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1};
  create_descriptor_set(ctx, 1, pool_sizes,
    &ctx->graphical_desc_layout, &ctx->graphical_desc_pool,
    &ctx->graphical_desc_set);

  /* Write image data to graphical descriptor set */
  create_image_view(ctx, format, VK_IMAGE_ASPECT_COLOR_BIT,
      &ctx->tex_image, &ctx->graphical_image_view);
  init_image_descriptor(ctx, &ctx->tex_image, 0, format,
    &ctx->sampler, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
    &ctx->graphical_image_view, &ctx->graphical_desc_set);

  /* Configure compute descriptor set layout */
  VkShaderStageFlagBits compute_stages[1] =
    {VK_SHADER_STAGE_COMPUTE_BIT};
  VkDescriptorType compute_types[1] =
    {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE};
  create_descriptor_set_layout(ctx, 1,
    compute_stages, compute_types, NULL, &ctx->compute_desc_layout);

  /* Configure compute descriptor set */
  VkDescriptorPoolSize compute_pool_sizes[1] =
    {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1};
  create_descriptor_set(ctx, 1, compute_pool_sizes,
    &ctx->compute_desc_layout, &ctx->compute_desc_pool,
    &ctx->compute_desc_set);

  /* Write image data to compute descriptor set */
  create_image_view(ctx, format, VK_IMAGE_ASPECT_COLOR_BIT,
      &ctx->tex_image, &ctx->compute_image_view);
  init_image_descriptor(ctx, &ctx->tex_image, 0, format,
    NULL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
    &ctx->compute_image_view, &ctx->compute_desc_set);
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
  destroy_image(ctx, &ctx->tex_image, &ctx->tex_image_memory);
  destroy_pipeline(ctx);
  destroy_compute_pipeline(ctx);
  destroy_descriptor_set(ctx, &ctx->graphical_desc_layout,
    &ctx->graphical_desc_pool, &ctx->graphical_desc_set);
  destroy_descriptor_set(ctx, &ctx->compute_desc_layout,
    &ctx->compute_desc_pool, &ctx->compute_desc_set);
  destroy_framebuffers(ctx);
  destroy_render_pass(ctx);
  destroy_sync(ctx);
  destroy_command_buffers(ctx, ctx->num_images,
    &ctx->command_pool, ctx->command_buffers);
  destroy_command_buffers(ctx, 1, &ctx->compute_command_pool, 
    &ctx->compute_command_buffer);  
  destroy_swapchain(ctx);
  destroy_surface(ctx);
  destroy_device(ctx);
}

