#ifndef CONTEXT_H_
#define CONTEXT_H_

#define NUM_QUEUES 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>

typedef struct ContextType {
  
  /* Fundamental structures */
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice phys_device;
  VkQueue queues[NUM_QUEUES];
  uint32_t family_indices[NUM_QUEUES];

  /* Profiling */
  PFN_vkDestroyDebugReportCallbackEXT destroy_func;
  VkDebugReportCallbackEXT callback;

  /* Command buffer */
  VkCommandPool command_pool;
  VkCommandBuffer command_buffer;  
  
} Context;

#endif /* CONTEXT_H_ */