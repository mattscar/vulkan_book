#ifndef SWAPCHAIN_UTILS_H_
#define SWAPCHAIN_UTILS_H_

#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>

#include "device_utils.h"
#include "surface_utils.h"

void create_swapchain(Context* ctx);
void destroy_swapchain(Context* ctx);

#endif /* SWAPCHAIN_UTILS_H_ */