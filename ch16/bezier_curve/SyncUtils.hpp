#ifndef SYNC_UTILS_H_
#define SYNC_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"

void createSync(Context& ctx);
void createDraw(Context& ctx, vk::PipelineStageFlags& stage);
void destroySync(Context& ctx);

#endif // SYNC_UTILS_H_ 