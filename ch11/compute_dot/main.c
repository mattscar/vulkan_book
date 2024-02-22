#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"
#include "command_utils.h"
#include "descriptor_utils.h"
#include "pipeline_utils.h"
#include "resource_utils.h"

#define NUM_VALUES 4096

/* Structure containing compute data */
typedef struct ComputeData_t {
  int32_t buffer_a[NUM_VALUES];
  int32_t buffer_b[NUM_VALUES];
  int32_t result[4];
  VkDispatchIndirectCommand indirect_data;
} ComputeData;

void destroy(Context* ctx);

int main() {
  Context ctx;

  /* Initialize data */
  uint32_t i;
  ComputeData compute_data;
  for(i=0; i<NUM_VALUES/4; i++) {
    compute_data.buffer_a[i] = 1;
    compute_data.buffer_b[i] = 1;
  }
  for(i=NUM_VALUES/4; i<NUM_VALUES/2; i++) {
    compute_data.buffer_a[i] = 1;
    compute_data.buffer_b[i] = 2;
  }
  for(i=NUM_VALUES/2; i<3*NUM_VALUES/4; i++) {
    compute_data.buffer_a[i] = 1;
    compute_data.buffer_b[i] = 3;
  }
  for(i=3*NUM_VALUES/4; i<NUM_VALUES; i++) {
    compute_data.buffer_a[i] = 1;
    compute_data.buffer_b[i] = 4;
  }
  for(i=0; i<4; i++) {
    compute_data.result[i] = -1;
  }
  compute_data.indirect_data = (VkDispatchIndirectCommand) {
    .x = 4,
    .y = 1,
    .z = 1
  };

  /* Create data structures */
  VkQueueFlagBits flags[NUM_QUEUES] =
    {VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Compute Dot");

  /* Create buffer containing compute data */
  create_buffer(&ctx,
    VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeof(compute_data),
    NULL, &ctx.compute_buffer_memory, &ctx.compute_buffer);

  /* Transfer buffer to device using staging buffer */
  transfer_to_device_buffer(&ctx, &compute_data,
    sizeof(compute_data), &ctx.compute_buffer);

  /* Create descriptor set layout */
  VkShaderStageFlagBits stages[2] =
    {VK_SHADER_STAGE_COMPUTE_BIT, VK_SHADER_STAGE_COMPUTE_BIT};
  VkDescriptorType types[2] =
    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
     VK_DESCRIPTOR_TYPE_STORAGE_BUFFER};
  create_descriptor_set_layout(&ctx, 2,
    stages, types, NULL, &ctx.descriptor_set_layout);
  
  /* Create descriptor set */
  VkDescriptorPoolSize pool_sizes[2] = {
    {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1},
    {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1}
  };    
  create_descriptor_set(&ctx, 2, pool_sizes, 
    &ctx.descriptor_set_layout, &ctx.descriptor_pool, 
    &ctx.descriptor_set);

  /* Associate buffer data with descriptor set bindings */
  init_buffer_descriptor(&ctx, &ctx.compute_buffer,
    0, 0, 2 * NUM_VALUES * sizeof(int32_t),
    VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, &ctx.descriptor_set);
  init_buffer_descriptor(&ctx, &ctx.compute_buffer,
    1, 2 * NUM_VALUES * sizeof(int32_t), sizeof(compute_data.result),
    VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &ctx.descriptor_set);

  /* Create pipeline and command buffer */
  create_pipeline(&ctx);
  VkCommandBufferBeginInfo command_begin_info =
    create_command_buffers(&ctx, &ctx.command_buffer,
      &ctx.command_pool, 1, COMPUTE_QTYPE,
      VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT);

  /* Record commands to the command buffer */
  ck(vkBeginCommandBuffer(ctx.command_buffer,
    &command_begin_info));

  /* Bind the descriptor set */
  vkCmdBindDescriptorSets(ctx.command_buffer,
    VK_PIPELINE_BIND_POINT_COMPUTE, ctx.pipeline_layout,
    0, 1, &ctx.descriptor_set, 0, NULL);

  /* Bind the compute pipeline to the command buffer */
  vkCmdBindPipeline(ctx.command_buffer,
    VK_PIPELINE_BIND_POINT_COMPUTE, ctx.pipeline);

  /* Record a command to perform computation */
  VkDeviceSize indirect_offset =
    (2 * NUM_VALUES + 4) * sizeof(int32_t);
  vkCmdDispatchIndirect(ctx.command_buffer,
    ctx.compute_buffer, indirect_offset);

  /* Finish recording commands to the command buffer */
  ck(vkEndCommandBuffer(ctx.command_buffer));

  /* Submit transfer command to queue */
  VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = 0,
    .pWaitSemaphores = NULL,
    .pWaitDstStageMask = NULL,
    .commandBufferCount = 1,
    .pCommandBuffers = &ctx.command_buffer,
    .signalSemaphoreCount = 0,
    .pSignalSemaphores = NULL
  };
  ck(vkQueueSubmit(ctx.queues[COMPUTE_QTYPE],
    1, &submit_info, NULL));

  /* Wait for transfer to complete */
  ck(vkQueueWaitIdle(ctx.queues[COMPUTE_QTYPE]));

  /* Read data from buffer */
  read_from_device_buffer(&ctx, 2 * NUM_VALUES * sizeof(int32_t),
    0, sizeof(compute_data.result), compute_data.result,
    &ctx.compute_buffer);

  /* Display results */
  for(i=0; i<4; i++) {
    printf("%d\n", compute_data.result[i]);
  }

  /* Deallocate resources */
  destroy_buffer(&ctx, &ctx.compute_buffer, &ctx.compute_buffer_memory);
  destroy_pipeline(&ctx);
  destroy_descriptor_set(&ctx, &ctx.descriptor_set_layout,
    &ctx.descriptor_pool, &ctx.descriptor_set);
  destroy_command_buffers(&ctx);
  destroy_device(&ctx);
  return 0;  
}

