#ifndef SYNC_UTILS_H_
#define SYNC_UTILS_H_

#include "context.h"
#include "error_utils.h"

void create_sync(Context *ctx);
void create_draw(Context *ctx, VkPipelineStageFlags *flags);
void destroy_sync(Context *ctx);

#endif /* SYNC_UTILS_H_ */