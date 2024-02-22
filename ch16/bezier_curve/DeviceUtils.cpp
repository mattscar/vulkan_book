#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"

#define VULKAN_HPP_NO_EXCEPTIONS

void DeviceUtils::createDevice(Context& ctx, 
  std::vector<vk::QueueFlagBits> queueFlags, std::string appName) {

  std::vector<vk::PhysicalDevice> devices;
  vk::PhysicalDeviceProperties deviceProps;
  std::vector<vk::QueueFamilyProperties> familyProps;
  std::vector<vk::DeviceQueueCreateInfo> queueInfos;
  int32_t familyIndex;

  // Set title
  ctx.title = appName;
  
  // Instance layers and extensions
  std::vector<const char*> instanceLayerNames;
  std::vector<const char*> instanceExtNames = {"VK_KHR_surface", "VK_KHR_xcb_surface"};
  std::vector<const char*> deviceLayerNames;
  std::vector<const char*> deviceExtNames = {"VK_KHR_swapchain"};

#ifdef DEBUG
  // Configure validation and error checking
  setupValidation(instanceLayerNames, instanceExtNames);
#endif

  // Application data structure
  vk::ApplicationInfo appInfo = vk::ApplicationInfo()
    .setPApplicationName(appName.c_str())
    .setApiVersion(VK_API_VERSION_1_0);

  vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
    .setPApplicationInfo(&appInfo)
    .setEnabledLayerCount(instanceLayerNames.size())
    .setPpEnabledLayerNames(instanceLayerNames.data())
    .setEnabledExtensionCount(instanceExtNames.size())
    .setPpEnabledExtensionNames(instanceExtNames.data());

  // Create an instance
  ctx.instance = vk::createInstance(instInfo);

#ifdef DEBUG
  // Configure validation and error checking
  setupReporting(ctx, instanceExtNames);
#endif

  // Get physical devices
  devices = ctx.instance.enumeratePhysicalDevices();

  // Iterate through devices to find a GPU
  for(vk::PhysicalDevice device: devices) {
    deviceProps = device.getProperties();
    if((deviceProps.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) || 
      (deviceProps.deviceType == vk::PhysicalDeviceType::eIntegratedGpu) || 
      (deviceProps.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)) {

      ctx.physDevice = device;
      break;
    }
  }

  // Exit if no physical device can be found
  if(!ctx.physDevice) {
    std::cerr << "Error: couldn't find GPU." << std::endl;
    exit(1);
  }

  // Obtain queue family properties
  familyProps = ctx.physDevice.getQueueFamilyProperties();

  // Iterate through queues
  uint32_t index = 0;
  for(vk::QueueFlagBits queueFlag: queueFlags) {

    // Find queue family that supports the given flag
    familyIndex = -1;
    for(uint32_t i=0; i<familyProps.size(); i++) {
      if(familyProps[i].queueFlags & queueFlag) {
        familyIndex = i;
        break;
      }
    }

    // Exit if no suitable queue family can be found
    if (familyIndex == -1) {
      std::cerr << "Error: couldn't find suitable queue family." << std::endl;
      exit(1);
    }
    ctx.familyIndices[index] = familyIndex;

    // Queue info
    queueInfos.push_back(vk::DeviceQueueCreateInfo()
      .setQueueFamilyIndex(ctx.familyIndices[index++])
      .setQueueCount(1));
  }

  // Device info
  vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo()
    .setQueueCreateInfoCount(queueInfos.size())
    .setPQueueCreateInfos(queueInfos.data())
    .setEnabledLayerCount(deviceLayerNames.size())
    .setPpEnabledLayerNames((const char* const*)deviceLayerNames.data())
    .setEnabledExtensionCount(deviceExtNames.size())
    .setPpEnabledExtensionNames((const char* const*)deviceExtNames.data());

  // Create logical device
  ctx.device = ctx.physDevice.createDevice(deviceInfo);

  // Access one queue for each family
  for(uint32_t i=0; i<queueFlags.size(); i++) {
    ctx.queues[i] = ctx.device.getQueue(ctx.familyIndices[i], 0);
  }
}

// Deallocate structures
void DeviceUtils::destroyDevice(Context& ctx) {

#ifdef DEBUG
  destroyReporting(ctx);
#endif

  ctx.device.destroy();  
  ctx.instance.destroy();
}