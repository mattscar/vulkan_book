#include "RenderPassUtils.hpp"

void createRenderPass(Context& ctx, std::vector<vk::ClearValue>& clearValues) {

  // Color attachment description 
  vk::AttachmentDescription colorDescription = vk::AttachmentDescription()
    .setFormat(ctx.imageFormat.format)
    .setSamples(vk::SampleCountFlagBits::e1)
    .setLoadOp(vk::AttachmentLoadOp::eClear)
    .setStoreOp(vk::AttachmentStoreOp::eStore)
    .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
    .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
    .setInitialLayout(vk::ImageLayout::eUndefined)
    .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

  // Color attachment reference 
  vk::AttachmentReference colorReference = vk::AttachmentReference()
    .setAttachment(0)
    .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

  // Subpass descriptions 
  vk::SubpassDescription subpassDescription = vk::SubpassDescription()
    .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
    .setInputAttachmentCount(0)
    .setPInputAttachments(nullptr)
    .setColorAttachmentCount(1)
    .setPColorAttachments(&colorReference);

  // Create render pass 
  vk::RenderPassCreateInfo renderPassCreateInfo = vk::RenderPassCreateInfo()
    .setAttachmentCount(1)
    .setPAttachments(&colorDescription)
    .setSubpassCount(1)
    .setPSubpasses(&subpassDescription)
    .setDependencyCount(0)
    .setPDependencies(nullptr);

  // Create the render pass 
  ctx.renderPass = ctx.device.createRenderPass(renderPassCreateInfo);

  // Set the render window
  vk::Rect2D rect = vk::Rect2D()
    .setOffset(vk::Offset2D(ctx.x, ctx.y))
    .setExtent(vk::Extent2D(ctx.width, ctx.height)); 
  
  // Configure info to begin renderPass 
  ctx.renderPassBeginInfo = vk::RenderPassBeginInfo()
    .setRenderPass(ctx.renderPass)
    .setRenderArea(rect) 
    .setClearValueCount(clearValues.size())
    .setPClearValues(clearValues.data());
}

// Deallocate the render pass 
void destroyRenderPass(Context& ctx) {
  ctx.device.destroyRenderPass(ctx.renderPass);
}