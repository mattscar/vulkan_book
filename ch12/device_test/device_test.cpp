#include <iostream>
#include <vulkan/vulkan.hpp>

int main() {

  vk::Instance instance;
  std::vector<vk::PhysicalDevice> devices;
  vk::PhysicalDeviceFeatures devFeatures;
  vk::PhysicalDeviceProperties props;
  std::vector<vk::QueueFamilyProperties> familyProps;
  uint32_t version, count;

  // Application data structure
  vk::ApplicationInfo appInfo = vk::ApplicationInfo()
    .setPApplicationName("Device test application")
    .setApiVersion(VK_API_VERSION_1_0);

  // Instance data structure */
  vk::InstanceCreateInfo instInfo = vk::InstanceCreateInfo()
    .setPApplicationInfo(&appInfo);
    
  // Create instance
  instance = vk::createInstance(instInfo);

  // Allocate and initialize memory for the physical devices
  devices = instance.enumeratePhysicalDevices();

  // Obtain the device's properties
  props = devices[0].getProperties();

  // Display device version and type
  version = props.apiVersion;
  std::cout << "Major version: " << ((version >> 22) & 0x3ff) << std::endl;
  std::cout << "Minor version: " << ((version >> 12) & 0x3ff) << std::endl;
  std::cout << "Patch version: " << (version & 0xfff) << std::endl;
  std::cout << "Type: " << to_string(props.deviceType) << std::endl;

  // Display the device's name
  std::cout << "Device name: " << props.deviceName << std::endl;

  // Identify whether the device supports sparse binding
  devFeatures = devices[0].getFeatures();
  if(devFeatures.sparseBinding)
    std::cout << "Sparse binding feature supported" << std::endl;
  else
    std::cout << "Sparse binding feature not supported" << std::endl;

  // Display the number of queue families available
  familyProps = devices[0].getQueueFamilyProperties();
  std::cout << "Number of queue families: " << familyProps.size() << std::endl;

  // Access a properties structure for each family */
  count = 0;
  for(vk::QueueFamilyProperties familyProp: familyProps) {
    
    std::cout << "Queue Family " << count++ << " has " << familyProp.queueCount << 
      " queues and supports the following operations: ";
    
    if(familyProp.queueFlags & vk::QueueFlagBits::eGraphics) {
      std::cout << "GRAPHICS ";
    }

    if(familyProp.queueFlags & vk::QueueFlagBits::eCompute) {
      std::cout << "COMPUTE ";
    }

    if(familyProp.queueFlags & vk::QueueFlagBits::eTransfer) {
      std::cout << "TRANSFER ";
    }

    if(familyProp.queueFlags & vk::QueueFlagBits::eSparseBinding) {
      std::cout << "SPARSE_BINDING ";
    }
    std::cout << std::endl;
  }
  
  // Deallocate the instance
  instance.destroy();

  return 0;
}