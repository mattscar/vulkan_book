#include "swapchain_utils.h"

void create_swapchain(Context* ctx) {

  VkExtent2D extent;
  uint32_t image_count;
  VkSurfaceCapabilitiesKHR surface_capabilities;

  /* Get surface characteristics */
  ck(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(ctx->phys_device, ctx->surface, &surface_capabilities));

  /* Get number of images */
  image_count = surface_capabilities.minImageCount + 1;
  if ((surface_capabilities.maxImageCount > 0) && (image_count > surface_capabilities.maxImageCount)) {
    image_count = surface_capabilities.maxImageCount;
  }

  /* Obtain the window's dimensions */
  extent = surface_capabilities.currentExtent;
  ctx->width = extent.width;
  ctx->height = extent.height;

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
    .imageExtent = extent,
    .imageArrayLayers = 1,
    .imageUsage =  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
    .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
    .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
    .presentMode = ctx->presentation_mode,
    .clipped = VK_TRUE,
    .oldSwapchain = old_swapchain
  };

  /* Create swapchain and get number of images */
  ck(vkCreateSwapchainKHR(ctx->device, &info, NULL, &ctx->swapchain));
  if(old_swapchain != NULL) {
    vkDestroySwapchainKHR(ctx->device, old_swapchain, NULL);
  }
  ck(vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, &ctx->num_images, NULL));

  /* Allocate memory and get images from swapchain */
  if(ctx->images != NULL) {
    free(ctx->images);
  }
  ctx->images = (VkImage*)malloc(ctx->num_images * sizeof(VkImage));
  ck(vkGetSwapchainImagesKHR(ctx->device, ctx->swapchain, &ctx->num_images, ctx->images));
}

void destroy_swapchain(Context* ctx) {

  free(ctx->images);
  vkDestroySwapchainKHR(ctx->device, ctx->swapchain, NULL);
}