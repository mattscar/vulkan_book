#ifndef SYNC_UTILS_H_
#define SYNC_UTILS_H_

#include "context.h"
#include "error_utils.h"

void set_image_barriers(Context *ctx, VkImageMemoryBarrier* image_barrier, VkImageMemoryBarrier* clear_barrier, uint32_t i);
void create_semaphores(Context *ctx);
void destroy_semaphores(Context *ctx);

#endif /* SYNC_UTILS_H_ */