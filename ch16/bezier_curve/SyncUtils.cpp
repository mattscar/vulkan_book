#include "SyncUtils.hpp"

void createSync(Context& ctx) {

  // Create semaphores for synchronizing commands
  vk::SemaphoreCreateInfo semaphoreInfo = vk::SemaphoreCreateInfo();
  ctx.acquireCompleteSemaphore = ctx.device.createSemaphore(semaphoreInfo);
  ctx.commandCompleteSemaphore = ctx.device.createSemaphore(semaphoreInfo);

  // Define range of image affected by barriers
  vk::ImageSubresourceRange subresourceRange =
    vk::ImageSubresourceRange()
    .setAspectMask(vk::ImageAspectFlagBits::eColor)
    .setBaseMipLevel(0)
    .setLevelCount(1)
    .setBaseArrayLayer(0)
    .setLayerCount(1);

  // Configure the barrier to set image layout before drawing
  ctx.drawBarrier = vk::ImageMemoryBarrier()
    .setSrcAccessMask(vk::AccessFlagBits::eMemoryRead)
    .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
    .setOldLayout(vk::ImageLayout::ePreinitialized)
    .setNewLayout(vk::ImageLayout::eColorAttachmentOptimal)
    .setSrcQueueFamilyIndex(0)
    .setDstQueueFamilyIndex(0)
    .setSubresourceRange(subresourceRange);

  // Configure the barrier to set image layout before presentation
  ctx.presentBarrier = vk::ImageMemoryBarrier()
    .setSrcAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
    .setDstAccessMask(vk::AccessFlagBits::eMemoryRead)
    .setOldLayout(vk::ImageLayout::eColorAttachmentOptimal)
    .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
    .setSrcQueueFamilyIndex(0)
    .setDstQueueFamilyIndex(0)
    .setSubresourceRange(subresourceRange);
}

void createDraw(Context& ctx, vk::PipelineStageFlags& stage) {

  // Configure the info for submitting commands to the queue
  ctx.submitInfo = vk::SubmitInfo()
    .setWaitSemaphoreCount(1)
    .setPWaitSemaphores(&ctx.acquireCompleteSemaphore)
    .setPWaitDstStageMask(&stage)
    .setCommandBufferCount(1)
    .setSignalSemaphoreCount(1)
    .setPSignalSemaphores(&ctx.commandCompleteSemaphore);

  // Configure the info for presenting images
  ctx.presentInfo = vk::PresentInfoKHR()
    .setWaitSemaphoreCount(1)
    .setPWaitSemaphores(&ctx.commandCompleteSemaphore)
    .setSwapchainCount(1)
    .setPSwapchains(&ctx.swapchain);
}

void destroySync(Context& ctx) {
  ctx.device.destroySemaphore(ctx.acquireCompleteSemaphore);
  ctx.device.destroySemaphore(ctx.commandCompleteSemaphore);
}