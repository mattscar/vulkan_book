#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "device_utils.h"

int main() {

  Context ctx;

  /* Create the device */
  VkQueueFlagBits flags[NUM_QUEUES] = {VK_QUEUE_GRAPHICS_BIT};
  create_device(&ctx, NUM_QUEUES, flags, "Queue Creation");

  /* Display the index of the family that supports graphics */
  for(uint32_t i=0; i<NUM_QUEUES; i++) {
    printf("Family %d supports graphics.\n", ctx.family_indices[i]);
  }

  /* Destroy the device */
  destroy_device(&ctx);

  return 0;
}