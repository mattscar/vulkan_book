#ifndef PIPELINE_UTILS_H_
#define PIPELINE_UTILS_H_

#include <fstream>
#include "Context.hpp"
#include "ErrorUtils.hpp"

namespace pipeline_utils {

void createPipeline(Context& ctx);

void destroyPipeline(Context& ctx);

}

#endif // PIPELINE_UTILS_H_ 