#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <vulkan/vulkan.h>

#define NUM_QUEUES 2
#define COMPUTE_QTYPE 0
#define TRANSFER_QTYPE 1

typedef struct ContextType {

  /* Fundamental structures */
  VkInstance instance;
  VkDevice device;
  VkPhysicalDevice phys_device;
  VkQueue queues[NUM_QUEUES];
  unsigned int family_indices[NUM_QUEUES];
  VkSubmitInfo submit_info;
  
  /* Profiling */
  PFN_vkDestroyDebugReportCallbackEXT destroy_func;
  VkDebugReportCallbackEXT callback;

  /* Command buffers and synchronization */
  VkCommandPool command_pool;
  VkCommandBuffer command_buffer;
  VkSemaphore acquire_complete_semaphore;
  VkSemaphore command_complete_semaphore;
  VkImageMemoryBarrier compute_barrier, present_barrier;
  VkPresentInfoKHR present_info;

  /* Buffers and descriptor sets */
  VkBuffer compute_buffer;
  VkDeviceMemory compute_buffer_memory;
  VkDescriptorSetLayout descriptor_set_layout;
  VkDescriptorPool descriptor_pool;
  VkDescriptorSet descriptor_set;

  /* Pipeline */
  char *compute_shader_code;
  VkShaderModule compute_module;
  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;
  
} Context;

#endif /* CONTEXT_H_ */