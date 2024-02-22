#ifndef SYNC_UTILS_H_
#define SYNC_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include "device_utils.h"

void create_sync(Context *ctx);
void create_draw(Context *ctx, VkPipelineStageFlags *flags);
void destroy_sync(Context *ctx);

#endif /* SYNC_UTILS_H_ */