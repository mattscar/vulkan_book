#ifndef PIPELINE_UTILS_H_
#define PIPELINE_UTILS_H_

#include <fstream>
#include <iterator>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "ErrorUtils.hpp"

void createPipeline(Context& ctx);
void destroyPipeline(Context& ctx);

#endif // PIPELINE_UTILS_H_ 