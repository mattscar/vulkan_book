#ifndef ERROR_UTILS_H_
#define ERROR_UTILS_H_

#include "context.h"

void ck(VkResult res);
void setup_validation(uint32_t* num_layers, char **layer_names, uint32_t* num_ext, char **ext_names);
void setup_reporting(Context* ctx, uint32_t num_ext, char **ext_names);
void destroy_reporting(Context* ctx);

#endif /* ERROR_UTILS_H_ */