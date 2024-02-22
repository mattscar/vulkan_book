#ifndef RENDER_UTILS_H_
#define RENDER_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "ErrorUtils.hpp"

void createRenderPass(Context& ctx, std::vector<vk::ClearValue>& clearValues);

void destroyRenderPass(Context& ctx);

#endif /* RENDER_UTILS_H_ */