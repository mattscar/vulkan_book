#include "context.h"
#include "command_utils.h"
#include "device_utils.h"
#include "resource_utils.h"

#define SIZE_SRC 32
#define SIZE_DST 16

int main() {
  Context ctx;
  VkBuffer buffer_src, buffer_dst;
  VkDeviceMemory mem_src, mem_dst;
  uint32_t i, data_src[SIZE_SRC], data_dst[SIZE_DST];
  void* output;

  /* Create the device */
  VkQueueFlagBits flags[NUM_QUEUES] = {VK_QUEUE_TRANSFER_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Buffer Demo");

  /* Create and initialize source buffer */
  for(i = 0; i < SIZE_SRC; i++) {
    data_src[i] = i;
  }
  create_buffer(&ctx, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    SIZE_SRC * sizeof(uint32_t), data_src, &mem_src, &buffer_src);

  /* Create and initialize destination buffer */
  for(i = 0; i < SIZE_DST; i++) {
    data_dst[i] = 0;
  }
  create_buffer(&ctx, VK_BUFFER_USAGE_TRANSFER_DST_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    SIZE_DST * sizeof(uint32_t), data_dst, &mem_dst, &buffer_dst);

  /* Create info for the copy operation */
  VkBufferCopy buffer_copy = {
    .srcOffset = 6 * sizeof(uint32_t),
    .dstOffset = 0,
    .size = SIZE_DST * sizeof(uint32_t)
  };

  /* Create command buffer and start recording */
  VkCommandBufferBeginInfo command_buffer_begin_info =
    create_command_buffer(&ctx);
  ck(vkBeginCommandBuffer(ctx.command_buffer, &command_buffer_begin_info));

  /* Record command to fill buffer */
  vkCmdCopyBuffer(ctx.command_buffer, buffer_src, buffer_dst, 1, &buffer_copy);

  /* End recording */
  ck(vkEndCommandBuffer(ctx.command_buffer));

  /* Submit buffer */
  VkSubmitInfo submit_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .commandBufferCount = 1,
    .pCommandBuffers = &ctx.command_buffer
  };
  ck(vkQueueSubmit(ctx.queues[0], 1, &submit_info, NULL));

  /* Wait for command buffer to finish */
  ck(vkQueueWaitIdle(ctx.queues[0]));

  /* Read and display output  */
  ck(vkMapMemory(ctx.device, mem_dst, 0, SIZE_DST * sizeof(uint32_t), 0, &output));
  printf("Output: ");
  for(i = 0; i < SIZE_DST; i++) {
    printf("%u ", ((uint32_t*)output)[i]);
  }
  printf("\n");
  vkUnmapMemory(ctx.device, mem_dst);

  /* Deallocate structures */
  destroy_buffer(&ctx, &buffer_src, &mem_src);
  destroy_buffer(&ctx, &buffer_dst, &mem_dst);
  destroy_command_buffer(&ctx);
  destroy_device(&ctx);
  return 0;
}