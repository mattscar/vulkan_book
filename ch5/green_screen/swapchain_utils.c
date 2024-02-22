#include "swapchain_utils.h"

void create_swapchain(Context* ctx) {
  VkSurfaceCapabilitiesKHR surface_capabilities;
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes, pref_mode;
  VkExtent2D extent;
  uint32_t i, num_formats, num_modes, image_count;
  
  /* Get surface characteristics */
  ck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->phys_device, 
    ctx->surface, &surface_capabilities));

  /* Get number of images */
  image_count = surface_capabilities.minImageCount + 1;
  if ((surface_capabilities.maxImageCount > 0) && 
      (image_count > surface_capabilities.maxImageCount)) {
    image_count = surface_capabilities.maxImageCount;
  }

  /* Obtain the window's dimensions */
  extent = surface_capabilities.currentExtent;
  ctx->width = extent.width;
  ctx->height = extent.height;

   /* Obtain image formats */
  ck(vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->phys_device, 
    ctx->surface, &num_formats, NULL));
  formats = malloc(num_formats * sizeof(VkSurfaceFormatKHR));
  ck(vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->phys_device, 
    ctx->surface, &num_formats, formats));

  /* Select 8-bit RGBA format if available */
  for(i = 0; i < num_formats; i++) {
    if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM) {
      ctx->image_format = formats[i];
      break;
    }
  }
  if (ctx->image_format.format != VK_FORMAT_R8G8B8A8_UNORM) {
    ctx->image_format = formats[0];
  }

  /* Get presentation modes */
  ck(vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->phys_device, 
    ctx->surface, &num_modes, NULL));
  modes = malloc(num_modes * sizeof(VkPresentModeKHR));
  ck(vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->phys_device, 
    ctx->surface, &num_modes, modes));
  
  /* Select mailbox mode if available */
  for(i = 0; i < num_modes; i++) {
    if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      pref_mode = modes[i];
      break;
    }
  }
  if (pref_mode != VK_PRESENT_MODE_MAILBOX_KHR) {
    pref_mode = VK_PRESENT_MODE_FIFO_KHR;
  }

  /* Swapchain info */
  VkSwapchainKHR old_swapchain = ctx->swapchain;
  VkSwapchainCreateInfoKHR info = {
    .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .surface = ctx->surface,
    .minImageCount = image_count,
    .imageFormat = ctx->image_format.format,
    .imageColorSpace = ctx->image_format.colorSpace,
    .imageExtent = {extent.width, extent.height},
    .imageArrayLayers = 1,
    .imageUsage =  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | 
      VK_IMAGE_USAGE_TRANSFER_DST_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 1,
    .pQueueFamilyIndices = NULL,
    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = pref_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_swapchain
  };
  
  /* Create swapchain and get number of images */
  ck(vkCreateSwapchainKHR(ctx->device, &info, NULL, &ctx->swapchain));
  if(old_swapchain != NULL) {
    vkDestroySwapchainKHR(ctx->device, old_swapchain, NULL);
  }
  ck(vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, 
    &ctx->num_images, NULL));
  
  /* Allocate memory and get images from swapchain */
  if(ctx->images != NULL) {
    free(ctx->images);
  }  
  ctx->images = malloc(ctx->num_images * sizeof(VkImage));  
  ck(vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, 
    &ctx->num_images, ctx->images));
  
  free(formats);
  free(modes);
}

void destroy_swapchain(Context* ctx) {
  free(ctx->images);
  vkDestroySwapchainKHR(ctx->device, ctx->swapchain, NULL); 
}