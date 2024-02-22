#ifndef DEVICE_UTILS_H_
#define DEVICE_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"

namespace device_utils {

void createDevice(Context& ctx, std::vector<vk::QueueFlagBits> queueFlags, 
  std::string appName);

void destroyDevice(Context& ctx);

}

#endif /* DEVICE_UTILS_H_ */