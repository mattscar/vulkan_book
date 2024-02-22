#ifndef FRAMEBUFFER_UTILS_H_
#define FRAMEBUFFER_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"
#include "ResourceUtils.hpp"

namespace framebuffer_utils {

void createFramebuffers(Context& ctx);

void destroyFramebuffers(Context& ctx);

}

#endif // FRAMEBUFFER_UTILS_H_ 