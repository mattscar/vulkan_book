#include "main.h"

/* Model matrix */
glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, -10.0));

/* Combining the view/persepctive matrix */
glm::vec3 viewer_pos = glm::vec3(-12.0f, -4.0f, -3.0f);
glm::mat4 view_matrix = glm::lookAt(viewer_pos, glm::vec3(0.0f, 0.0f, -14.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 projection_matrix = glm::perspective(glm::radians(40.0f), 1.33f, 5.0f, 25.0f);

/* Initialize draw data */
DrawData draw_data = {
  .vertex_data = {
    // Left face
    {{-1.0, -1.0, -1.0}, {0.0, 0.0, 0.2}, {-1.0,  0.0,  0.0}},
    {{-1.0,  1.0, -1.0}, {0.0, 0.0, 0.2}, {-1.0,  0.0,  0.0}},
    {{-1.0,  1.0,  1.0}, {0.0, 0.0, 0.2}, {-1.0,  0.0,  0.0}},
    {{-1.0, -1.0,  1.0}, {0.0, 0.0, 0.2}, {-1.0,  0.0,  0.0}},
    // Right face
    {{ 1.0, -1.0, -1.0}, {0.2, 0.2, 0.0}, { 1.0,  0.0,  0.0}},
    {{ 1.0,  1.0, -1.0}, {0.2, 0.2, 0.0}, { 1.0,  0.0,  0.0}},
    {{ 1.0,  1.0,  1.0}, {0.2, 0.2, 0.0}, { 1.0,  0.0,  0.0}},
    {{ 1.0, -1.0,  1.0}, {0.2, 0.2, 0.0}, { 1.0,  0.0,  0.0}},
    // Top face
    {{-1.0, -1.0, -1.0}, {0.0, 0.2, 0.0}, { 0.0, -1.0,  0.0}},
    {{ 1.0, -1.0, -1.0}, {0.0, 0.2, 0.0}, { 0.0, -1.0,  0.0}},
    {{ 1.0, -1.0,  1.0}, {0.0, 0.2, 0.0}, { 0.0, -1.0,  0.0}},
    {{-1.0, -1.0,  1.0}, {0.0, 0.2, 0.0}, { 0.0, -1.0,  0.0}},
    // Bottom face
    {{-1.0,  1.0, -1.0}, {0.2, 0.0, 0.2}, { 0.0,  1.0,  0.0}},
    {{ 1.0,  1.0, -1.0}, {0.2, 0.0, 0.2}, { 0.0,  1.0,  0.0}},
    {{ 1.0,  1.0,  1.0}, {0.2, 0.0, 0.2}, { 0.0,  1.0,  0.0}},
    {{-1.0,  1.0,  1.0}, {0.2, 0.0, 0.2}, { 0.0,  1.0,  0.0}},
    // Front face
    {{-1.0, -1.0,  1.0}, {0.2, 0.0, 0.0}, { 0.0,  0.0,  1.0}},
    {{ 1.0, -1.0,  1.0}, {0.2, 0.0, 0.0}, { 0.0,  0.0,  1.0}},
    {{ 1.0,  1.0,  1.0}, {0.2, 0.0, 0.0}, { 0.0,  0.0,  1.0}},
    {{-1.0,  1.0,  1.0}, {0.2, 0.0, 0.0}, { 0.0,  0.0,  1.0}},
    // Rear face
    {{-1.0, -1.0, -1.0}, {0.0, 0.2, 0.2}, { 0.0,  0.0, -1.0}},
    {{ 1.0, -1.0, -1.0}, {0.0, 0.2, 0.2}, { 0.0,  0.0, -1.0}},
    {{ 1.0,  1.0, -1.0}, {0.0, 0.2, 0.2}, { 0.0,  0.0, -1.0}},
    {{-1.0,  1.0, -1.0}, {0.0, 0.2, 0.2}, { 0.0,  0.0, -1.0}}
  },
  .index_data = {
    0, 1, 2, 3, 0xFFFF,
    7, 6, 5, 4, 0xFFFF,
    11, 10, 9, 8, 0xFFFF,
    12, 13, 14, 15, 0xFFFF,
    19, 18, 17, 16, 0xFFFF,
    20, 21, 22, 23, 0xFFFF
  },
  .indirect_data = (VkDrawIndexedIndirectCommand) {
    .indexCount = NUM_INDICES,
    .instanceCount = NUM_INSTANCES,
    .firstIndex = 0,
    .vertexOffset = 0,
    .firstInstance = 0
  },
  .vertex_uniform_data = {
    .model_matrix = model_matrix,
    .view_matrix = view_matrix,
    .projection_matrix = projection_matrix,
    .light_pos = {-5.0, -5.0, -20.0},
    .viewer_pos = viewer_pos,
  },
  .fragment_uniform_data = {
    .light_params = {
      {0.05, 0.05, 0.05}, {0.1, 0.1, 0.1}, {0.05, 0.05, 0.05}
    },
    .shininess = 0.1
  }
};

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
  ctx.swapchain = NULL;
  ctx.command_buffers = NULL;  
  ctx.images = NULL;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;
  ctx.depth_test = VK_TRUE;
  VkDescriptorSetLayoutBinding set_bindings[NUM_DESCRIPTOR_BINDINGS];  

  /* Clear values for the framebuffers */
  uint32_t num_clear_values = (ctx.depth_test) ? 2 : 1;
  VkClearValue clear_values[2];
  clear_values[0].color = { { 0.25f, 0.25f, 0.25f, 0.25f } };
  clear_values[1].depthStencil = { 0.83f, 0 };

  /* Set dynamic state variables */
  ctx.viewport = {
    .x = 0, .y = 0,
    .width = ctx.width * 1.0f,
    .height = ctx.height * 1.0f,
    .minDepth = 0.0f, .maxDepth = 1.0f
  };
  ctx.scissor = {
    .offset = { .x = 0, .y = 0 },
    .extent = {
      .width = ctx.width,
      .height = ctx.height
    }
  };

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] = 
    {VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Two Cubes");  
  create_surface(&ctx);
  create_sync(&ctx);
  create_render_pass(&ctx, num_clear_values, clear_values);  

  /* Create device buffer */
  create_buffer(&ctx,
    VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
    VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(draw_data),
    NULL, &ctx.draw_buffer_memory, &ctx.draw_buffer);

  /* Set buffer offsets */
  VkDeviceSize vertex_uniform_offset = sizeof(draw_data.vertex_data) +
    sizeof(draw_data.index_data) + sizeof(draw_data.indirect_data);
  VkDeviceSize fragment_uniform_offset =
    vertex_uniform_offset + sizeof(draw_data.vertex_uniform_data);

  /* Transfer data from staging buffer to device buffer */
  transfer_to_device_buffer(&ctx, &draw_data, sizeof(draw_data), 
    &ctx.draw_buffer);

  /* Configure uniforms */
  VkDescriptorPoolSize pool_sizes[1] = {
    {
      .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
      .descriptorCount = 2
    }
  };
  VkShaderStageFlagBits stages[NUM_DESCRIPTOR_BINDINGS] =
    {VK_SHADER_STAGE_VERTEX_BIT, VK_SHADER_STAGE_FRAGMENT_BIT};
  create_descriptor_set(&ctx, NUM_DESCRIPTOR_BINDINGS, pool_sizes,
    stages, set_bindings, NULL);
  init_uniform_descriptor(&ctx, &ctx.draw_buffer, 0,
    vertex_uniform_offset, sizeof(draw_data.vertex_uniform_data));
  init_uniform_descriptor(&ctx, &ctx.draw_buffer, 1,
    fragment_uniform_offset, sizeof(draw_data.fragment_uniform_data));    
  
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
  
  /* Initialize structures */     
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
  
  /* Update rendering dimensions */
  ctx->render_pass_begin_info.renderArea.extent.width = ctx->width;
  ctx->render_pass_begin_info.renderArea.extent.height = ctx->height;    
  
  /* Set buffer offsets */
  VkDeviceSize vertex_offset = 0;
  VkDeviceSize index_offset = NUM_VERTICES * sizeof(Vertex);
  VkDeviceSize indirect_offset = index_offset + NUM_INDICES * sizeof(uint16_t);

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

    /* Set the viewport */
    ctx->viewport.width = ctx->width * 1.0f;
    ctx->viewport.height = ctx->width * 1.0f;    
    vkCmdSetViewport(ctx->command_buffers[i], 0, 1,
      &ctx->viewport);

    /* Set the scissor dimensions */
    ctx->scissor.extent.width = ctx->width * 1.0f;
    ctx->scissor.extent.height = ctx->width * 1.0f;    
    vkCmdSetScissor(ctx->command_buffers[i], 0, 1,
      &ctx->scissor);  
  
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

    /* Prepare images for presentation */
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

