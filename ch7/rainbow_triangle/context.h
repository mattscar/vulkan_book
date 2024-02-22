#ifndef CONTEXT_H_
#define CONTEXT_H_

#define NUM_QUEUES 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>

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
  uint32_t family_indices[NUM_QUEUES];
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

  /* Buffers */
  VkBuffer vertex_buffer;
  VkDeviceMemory vertex_buffer_memory;

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

  /* Pipeline */
  char *vertex_shader_code, *fragment_shader_code;
  VkShaderModule vertex_module, fragment_module;
  VkPipelineLayout pipeline_layout;
  VkPipeline pipeline;

} Context;

#endif /* CONTEXT_H_ */