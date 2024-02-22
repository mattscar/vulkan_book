#ifndef FRAMEBUFFER_UTILS_H_
#define FRAMEBUFFER_UTILS_H_

#include <vulkan/vulkan.h>
#include "device_utils.h"
#include "error_utils.h"

void create_framebuffers(Context* ctx);

void destroy_framebuffers(Context* ctx);

#endif /* FRAMEBUFFER_UTILS_H_ */