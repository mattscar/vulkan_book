#ifndef COMPUTE_UTILS_H_
#define COMPUTE_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "context.h"
#include "error_utils.h"

extern uint32_t read_binary_file(const char* filename, char** code);

void create_compute_pipeline(Context* ctx);

void destroy_compute_pipeline(Context* ctx);

#endif /* COMPUTE_UTILS_H_ */