#ifndef CONTEXT_H_
#define CONTEXT_H_

#include <vulkan/vulkan.h>

#define NUM_QUEUES 3
#define GRAPHICS_QTYPE 0
#define TRANSFER_QTYPE 1
#define COMPUTE_QTYPE 2

#ifdef VK_USE_PLATFORM_WIN32_KHR
typedef HWND NativeWindow;
typedef HINSTANCE NativeDisplay;

#elif VK_USE_PLATFORM_XCB_KHR
typedef xcb_window_t NativeWindow;
typedef xcb_connection_t* NativeDisplay;

#elif VK_USE_PLATFORM_ANDROID_KHR
typedef ANativeWindow* NativeWindow;
typedef int NativeDisplay;

#elif VK_USE_PLATFORM_WAYLAND_KHR
typedef wl_surface* NativeWindow;
typedef wl_display* NativeDisplay;

#elif VK_USE_PLATFORM_XLIB_KHR
typedef Window NativeWindow;
typedef Display* NativeDisplay;
#endif

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
  VkCommandBuffer* command_buffers;
  VkSemaphore acquire_complete_semaphore;
  VkSemaphore command_complete_semaphore;
  VkImageMemoryBarrier draw_barrier, present_barrier;
  VkPresentInfoKHR present_info;

  /* Buffers and descriptor sets */
  VkBuffer draw_buffer;
  VkDeviceMemory draw_buffer_memory;
  VkDescriptorSetLayout graphical_desc_layout, compute_desc_layout;
  VkDescriptorPool graphical_desc_pool, compute_desc_pool;
  VkDescriptorSet graphical_desc_set, compute_desc_set;
  VkImageView graphical_image_view, compute_image_view;

  /* Surface and swapchain */
  NativeWindow window;
  NativeDisplay display;
  VkSurfaceKHR surface;
  VkSurfaceFormatKHR image_format;
  VkPresentModeKHR presentation_mode;
  const char* title;
  uint32_t x, y, width, height;
  uint32_t num_images;
  VkSwapchainKHR swapchain;
  VkImage* images;

  /* Framebuffers and render pass */
  VkImageView* image_views;
  VkFramebuffer* framebuffers;
  VkRenderPass render_pass;
  VkRenderPassBeginInfo render_pass_begin_info;

  /* Graphical pipeline */
  char *vertex_shader_code, *fragment_shader_code;
  VkShaderModule vertex_module, fragment_module;
  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;

  /* Compute pipeline and command buffer */
  char *compute_shader_code;
  VkShaderModule compute_module;
  VkPipelineLayout compute_pipeline_layout;
  VkPipeline compute_pipeline;
  VkCommandPool compute_command_pool;
  VkCommandBuffer compute_command_buffer;

  /* Texture */
  VkImage tex_image;
  VkDeviceMemory tex_image_memory;
  VkSampler sampler;

} Context;

#endif /* CONTEXT_H_ */