#ifndef RENDER_UTILS_H_
#define RENDER_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"

namespace renderpass_utils {

void createRenderPass(Context& ctx, std::vector<vk::ClearValue>& clearValues);

void destroyRenderPass(Context& ctx);

}

#endif /* RENDER_UTILS_H_ */