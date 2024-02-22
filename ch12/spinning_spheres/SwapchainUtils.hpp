#ifndef SWAPCHAIN_UTILS_H_
#define SWAPCHAIN_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"

namespace swapchain_utils {

void createSwapchain(Context& ctx);

void destroySwapchain(Context& ctx);

}

#endif // SWAPCHAIN_UTILS_H_ 