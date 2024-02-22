#ifndef CONTEXT_H_
#define CONTEXT_H_

#define NUM_QUEUES 2
#define GRAPHICS_QTYPE 0
#define TRANSFER_QTYPE 1

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
  
  // Fundamental structures 
  vk::Instance instance;
  vk::Device device;
  vk::PhysicalDevice physDevice = nullptr;
  vk::Queue queues[NUM_QUEUES];
  uint32_t familyIndices[NUM_QUEUES];
  vk::SubmitInfo submitInfo;
  
  // Profiling 
  PFN_vkDestroyDebugReportCallbackEXT destroyFunc;
  VkDebugReportCallbackEXT callback;

  // Command buffers and synchronization 
  vk::CommandPool commandPool;
  std::vector<vk::CommandBuffer> commandBuffers;
  vk::Semaphore acquireCompleteSemaphore;
  vk::Semaphore commandCompleteSemaphore;
  vk::ImageMemoryBarrier drawBarrier, presentBarrier;
  vk::PresentInfoKHR presentInfo;

  // Buffers and descriptor sets 
  vk::Buffer drawBuffer;
  vk::DeviceMemory drawBufferMemory;

  // Surface and swapchain 
  NativeWindow window;
  NativeDisplay display;
  vk::SurfaceKHR surface;
  vk::SurfaceFormatKHR imageFormat;
  vk::PresentModeKHR presentationMode;
  std::string title;
  uint32_t x, y, width, height;
  uint32_t numImages;
  vk::SwapchainKHR swapchain;
  std::vector<vk::Image> images;

  // Framebuffers and render pass 
  std::vector<vk::ImageView> imageViews;
  std::vector<vk::Framebuffer> framebuffers;
  vk::RenderPass renderPass;
  vk::RenderPassBeginInfo renderPassBeginInfo;

  // Pipeline 
  vk::ShaderModule vertexModule, fragmentModule, tescModule, teseModule;
  vk::PipelineLayout pipelineLayout;
  vk::Pipeline pipeline;  
  
} Context;

#endif // CONTEXT_H_ 