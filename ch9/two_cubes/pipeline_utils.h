#ifndef PIPELINE_UTILS_H_
#define PIPELINE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>
#include "device_utils.h"
#include "error_utils.h"

void create_pipeline(Context* ctx);
void destroy_pipeline(Context* ctx);

#endif /* PIPELINE_UTILS_H_ */