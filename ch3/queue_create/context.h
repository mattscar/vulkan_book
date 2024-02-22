#ifndef CONTEXT_H_
#define CONTEXT_H_

#define NUM_QUEUES 1

typedef struct ContextType {
  
  /* Fundamental structures */
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice phys_device;
  VkQueue queues[NUM_QUEUES];
  unsigned int family_indices[NUM_QUEUES];

  /* Profiling */
  PFN_vkDestroyDebugReportCallbackEXT destroy_func;
  VkDebugReportCallbackEXT callback;

} Context;

#endif /* CONTEXT_H_ */