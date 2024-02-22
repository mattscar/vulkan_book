#ifndef GLI_UTILS_H_
#define GLI_UTILS_H_

#include <stdio.h>

#include "gli/format.hpp"

#include <vulkan/vulkan.h>

VkFormat convert_format(gli::format gli_format);

#endif /* GLI_UTILS_H_ */