#ifndef SWAPCHAIN_UTILS_H_
#define SWAPCHAIN_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "SurfaceUtils.hpp"

void createSwapchain(Context& ctx);
void destroySwapchain(Context& ctx);

#endif // SWAPCHAIN_UTILS_H_ 