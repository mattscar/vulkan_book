#ifndef RENDER_UTILS_H_
#define RENDER_UTILS_H_

#include <vulkan/vulkan.h>
#include "device_utils.h"
#include "error_utils.h"

void create_render_pass(Context* ctx,
  uint32_t num_clear_values, VkClearValue* clear_values);

void destroy_render_pass(Context* ctx);

#endif /* RENDER_UTILS_H_ */