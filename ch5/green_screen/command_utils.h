#ifndef COMMAND_UTILS_H_
#define COMMAND_UTILS_H_

#include "context.h"
#include "error_utils.h"

VkCommandBufferBeginInfo create_command_buffers(Context *ctx);

void destroy_command_buffers(Context *ctx);

#endif /* COMMAND_UTILS_H_ */