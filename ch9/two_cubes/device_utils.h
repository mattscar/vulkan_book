#ifndef DEVICE_UTILS_H_
#define DEVICE_UTILS_H_

#include "context.h"
#include "error_utils.h"
#include <vulkan/vulkan.h>

void create_device(Context* ctx, uint32_t num_queues, 
  VkQueueFlagBits queue_flags[], const char* app_name);

void destroy_device(Context* ctx);

#endif /* DEVICE_UTILS_H_ */