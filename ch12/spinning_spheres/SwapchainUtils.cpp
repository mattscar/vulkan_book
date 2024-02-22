#include "SwapchainUtils.hpp"

namespace swapchain_utils {

void createSwapchain(Context& ctx) {

  vk::Extent2D extent;
  uint32_t imageCount;
  
  // Get surface characteristics 
  auto capRes = ctx.physDevice.getSurfaceCapabilitiesKHR(ctx.surface);
  ck(capRes.result);
  vk::SurfaceCapabilitiesKHR surfaceCapabilities = capRes.value;

  // Get number of images 
  imageCount = surfaceCapabilities.minImageCount + 1;
  if ((surfaceCapabilities.maxImageCount > 0) && 
    (imageCount > surfaceCapabilities.maxImageCount)) {

    imageCount = surfaceCapabilities.maxImageCount;
  }

  // Obtain the window's dimensions 
  extent = surfaceCapabilities.currentExtent;
  ctx.width = extent.width;
  ctx.height = extent.height;
  
  // Swapchain info 
  vk::SwapchainKHR oldSwapchain = ctx.swapchain;  
  vk::SwapchainCreateInfoKHR info = vk::SwapchainCreateInfoKHR()
    .setSurface(ctx.surface)
    .setMinImageCount(imageCount)
    .setImageFormat(ctx.imageFormat.format)
    .setImageColorSpace(ctx.imageFormat.colorSpace)
    .setImageExtent(extent)
    .setImageArrayLayers(1)
    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
    .setImageSharingMode(vk::SharingMode::eExclusive)
    .setQueueFamilyIndexCount(0)
    .setPQueueFamilyIndices(NULL)
    .setPreTransform(vk::SurfaceTransformFlagBitsKHR::eIdentity)
    .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
    .setPresentMode(ctx.presentationMode)
    .setClipped(VK_TRUE)
    .setOldSwapchain(oldSwapchain);
  
  // Create swapchain
  auto swapRes = ctx.device.createSwapchainKHR(info);
  ck(swapRes.result);
  ctx.swapchain = swapRes.value;
  
  // Delete old swapchain if necessary
  if(oldSwapchain) {
    for(vk::ImageView imgView: ctx.imageViews) {
      ctx.device.destroyImageView(imgView);
    }
    ctx.device.destroySwapchainKHR(oldSwapchain);
  }

  // Get images from swapchain
  auto imgRes = ctx.device.getSwapchainImagesKHR(ctx.swapchain);
  ck(imgRes.result);
  ctx.images = imgRes.value;
  ctx.numImages = ctx.images.size();
}

void destroySwapchain(Context& ctx) {
  ctx.device.destroySwapchainKHR(ctx.swapchain);
}

}