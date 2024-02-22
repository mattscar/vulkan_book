#include "FramebufferUtils.hpp"

void createFramebuffers(Context& ctx) {
  
  // Allocate memory for image views
  ctx.imageViews.resize(ctx.numImages);
  
  // Create image views and framebuffers 
  for(uint32_t i=0; i<ctx.numImages; ++i) {
  
    // Create an image view 
    createImageView(ctx, ctx.imageFormat.format, 
      vk::ImageAspectFlagBits::eColor, 
      ctx.images[i], ctx.imageViews[i]);

    // Create a framebuffer 
    vk::FramebufferCreateInfo framebufferCreateInfo = vk::FramebufferCreateInfo()
      .setRenderPass(ctx.renderPass)
      .setAttachmentCount(1)
      .setPAttachments(&ctx.imageViews[i])
      .setWidth(ctx.width)
      .setHeight(ctx.height)
      .setLayers(1);
    ctx.framebuffers.push_back(
      ctx.device.createFramebuffer(framebufferCreateInfo));
  }
}

// Deallocate the render pass 
void destroyFramebuffers(Context& ctx) {

  // Destroy image views and framebuffers 
  for(uint32_t i=0; i<ctx.images.size(); ++i) {
    ctx.device.destroyFramebuffer(ctx.framebuffers[i]);
    ctx.device.destroyImageView(ctx.imageViews[i]);
  }
}