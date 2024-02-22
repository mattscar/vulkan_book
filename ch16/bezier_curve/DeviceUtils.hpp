#ifndef DEVICE_UTILS_H_
#define DEVICE_UTILS_H_

#define VULKAN_HPP_NO_EXCEPTIONS

#include "Context.hpp"
#include "ErrorUtils.hpp"

#include <vulkan/vulkan.hpp>

class DeviceUtils {
 
  public:
    static void createDevice(Context& ctx, std::vector<vk::QueueFlagBits> queueFlags, 
      std::string appName);

    static void destroyDevice(Context& ctx);
      
  private:
    DeviceUtils() {}
};

#endif /* DEVICE_UTILS_H_ */