#include <stdio.h>
#include <stdlib.h>

#include <vulkan/vulkan.h>

int main() {
  VkInstance instance;
  VkPhysicalDevice* devices;
  VkPhysicalDeviceFeatures dev_features;
  VkPhysicalDeviceProperties props;
  VkQueueFamilyProperties* family_props;
  VkResult result;
  unsigned int version, dev_count, family_count, index;
  char* devName;

  /* Application data structure */
  VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = "Device test application",
    .applicationVersion = 1,
    .pEngineName = NULL,
    .engineVersion = 1,
    .apiVersion = VK_MAKE_VERSION(1, 0, 3)
  };

  /* Instance data structure */
  const char* my_layer = "VK_LAYER_SCARPINO_helloworld";  
  VkInstanceCreateInfo inst_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &app_info,
    .enabledLayerCount = 1,
    .ppEnabledLayerNames = &my_layer,
    .enabledExtensionCount = 0,
    .ppEnabledExtensionNames = NULL
  };

  /* Create the instance */
  result = vkCreateInstance(&inst_info, NULL, &instance);
  if (result != VK_SUCCESS) {
    fprintf(stderr, "Failed to create instance: %d\n", result);
    exit(EXIT_FAILURE);
  }

  /* Determine the number of connected devices */
  result = vkEnumeratePhysicalDevices(instance, &dev_count, NULL);
  if (result != VK_SUCCESS) {
    fprintf(stderr, "Failed to obtain the number of physical devices: %d\n", result);
    exit(EXIT_FAILURE);
  }

  /* Allocate and initialize memory for the physical devices */
  devices = (VkPhysicalDevice*)malloc(dev_count * sizeof(VkPhysicalDevice));
  vkEnumeratePhysicalDevices(instance, &dev_count, devices);

  /* Obtain the device's properties */
  vkGetPhysicalDeviceProperties(devices[0], &props);

  /* Display device version and type */
  version = props.apiVersion;
  printf("Major version: %d\n", (version >> 22) & 0x3ff);
  printf("Minor version: %d\n", (version >> 12) & 0x3ff);
  printf("Patch version: %d\n", version & 0xfff);
  printf("Type: %d\n", props.deviceType);

  /* Display the device's name */
  devName = props.deviceName;
  printf("Device name: %s\n", devName);

  /* Identify whether the device supports sparse binding */
  vkGetPhysicalDeviceFeatures(devices[0], &dev_features);
  if(dev_features.sparseBinding)
    printf("Sparse binding feature supported\n");
  else
    printf("Sparse binding feature not supported\n");

  /* Display the number of queue families available */
  vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &family_count, NULL);
  printf("Number of queue families: %d\n", family_count);

  /* Access a properties structure for each family */
  family_props = (VkQueueFamilyProperties*)malloc(family_count * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &family_count, family_props);

  /* Display supported operations for each queue family */
  for(index = 0; index < family_count; index++) {

    printf("Queue family %u has %u queues and supports the following operations: ",
      index, family_props[index].queueCount);
    if(family_props[index].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      printf("GRAPHICS ");
    }
    if(family_props[index].queueFlags & VK_QUEUE_COMPUTE_BIT) {
      printf("COMPUTE ");
    }
    if(family_props[index].queueFlags & VK_QUEUE_TRANSFER_BIT) {
      printf("TRANSFER ");
    }
    if(family_props[index].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
      printf("SPARSE_BINDING ");
    }
    printf("\n");
  }

  /* Deallocate instance and device memory */
  free(family_props);
  free(devices);
  vkDestroyInstance(instance, NULL);
  return 0;
}