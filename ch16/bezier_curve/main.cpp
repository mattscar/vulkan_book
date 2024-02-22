#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "CommandUtils.hpp"
#include "PipelineUtils.hpp"
#include "RenderPassUtils.hpp"
#include "FramebufferUtils.hpp"
#include "ResourceUtils.hpp"
#include "SurfaceUtils.hpp"
#include "SwapchainUtils.hpp"
#include "SyncUtils.hpp"

#define NUM_VERTICES 4

// Structure for vertex data
typedef struct Vertex_t {
  float position[2];
  float color[3];
} Vertex;

// Structure for all draw data
typedef struct DrawData_t {
  Vertex vertexData[NUM_VERTICES];
  vk::DrawIndirectCommand indirectData;
} DrawData;

void recordCommands(Context& ctx);

// Submit the command buffer and present the processed image
void draw(Context& ctx) {

  // Acquire the next image
  vk::ResultValue<uint32_t> res = ctx.device.acquireNextImageKHR(ctx.swapchain,
    UINT64_MAX, ctx.acquireCompleteSemaphore, nullptr);
  if(res.result == vk::Result::eErrorOutOfDateKHR) {
    return;
  }

  // Submit the command buffer to the queue
  uint32_t index = res.value;
  ctx.submitInfo.pCommandBuffers = &ctx.commandBuffers[index];
  ctx.queues[GRAPHICS_QTYPE].submit(ctx.submitInfo, nullptr);

  // Present the image
  ctx.presentInfo.pImageIndices = &index;
  ctx.queues[GRAPHICS_QTYPE].presentKHR(ctx.presentInfo);
}

int main() {

  Context ctx;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;

  vk::ClearColorValue color(std::array<float, 4>{0.25f, 0.25f, 0.25f, 0.25f});
  std::vector<vk::ClearValue> clearValues;
  clearValues.push_back(vk::ClearValue().setColor(color));

  // Initialize draw data
  DrawData drawData = {
    .vertexData = { {{-0.25, -0.25}, {1.0, 1.0, 1.0}},
                     {{ 0.25, -0.25}, {1.0, 1.0, 1.0}},
                     {{ 0.25,  0.25}, {1.0, 1.0, 1.0}},
                     {{-0.25,  0.25}, {1.0, 1.0, 1.0}} },
    .indirectData = (vk::DrawIndirectCommand) {
      .vertexCount = NUM_VERTICES,
      .instanceCount = 1,
      .firstVertex = 0,
      .firstInstance = 0
    }
  };

  // Create data structures
  std::vector<vk::QueueFlagBits> flags =
    {vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eTransfer};
  DeviceUtils::createDevice(ctx, flags, std::string("Bezier Curve"));
  createSurface(ctx);
  createSync(ctx);
  createRenderPass(ctx, clearValues);

  // Create device buffer
  createBuffer(ctx,
    vk::BufferUsageFlagBits::eVertexBuffer 
    | vk::BufferUsageFlagBits::eIndirectBuffer 
    | vk::BufferUsageFlagBits::eTransferDst,
    vk::MemoryPropertyFlagBits::eDeviceLocal, sizeof(drawData),
    NULL, ctx.drawBufferMemory, ctx.drawBuffer);

  // Transfer data from staging buffer to device buffer
  transferToDeviceBuffer(ctx, &drawData, sizeof(drawData), ctx.drawBuffer);

  // Create pipeline
  createPipeline(ctx);

  // Start recording commands
  recordCommands(ctx);
  return 0;
}

void recordCommands(Context& ctx) {

  uint32_t count = 0;
  vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eTransfer;
  vk::CommandBufferBeginInfo commandBeginInfo;

  // Initialize push constant data
  float pushConstantData[2] = {1.0, 4.0};

  // Create structures
  createSwapchain(ctx);

  if(ctx.commandBuffers.empty()) {

    commandBeginInfo =
      createCommandBuffers(ctx, ctx.commandBuffers,
        ctx.commandPool, ctx.numImages, GRAPHICS_QTYPE,
        vk::CommandBufferUsageFlagBits::eSimultaneousUse);
  }

  createFramebuffers(ctx);
  createDraw(ctx, stage);

  // Set buffer offsets
  vk::DeviceSize vertexOffset = 0;
  vk::DeviceSize indirectOffset = NUM_VERTICES * sizeof(Vertex);

  // Record commands to the command buffer
  for(vk::CommandBuffer commandBuffer: ctx.commandBuffers) {

    // Record commands to the command buffer
    commandBuffer.begin(commandBeginInfo);

    // Bind the vertex buffer to the command buffer
    commandBuffer.bindVertexBuffers(0, 1, &ctx.drawBuffer, &vertexOffset);

    // Bind push constant data to the command buffer
    commandBuffer.pushConstants(ctx.pipelineLayout,
      vk::ShaderStageFlagBits::eTessellationControl,
      0, 2*sizeof(float), pushConstantData);

    // Record commands to the render pass
    ctx.renderPassBeginInfo.framebuffer = ctx.framebuffers[count++];
    commandBuffer.beginRenderPass(ctx.renderPassBeginInfo, vk::SubpassContents::eInline);

    // Bind the graphics pipeline to the command buffer
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, ctx.pipeline);

    // Perform the draw operation
    commandBuffer.drawIndirect(ctx.drawBuffer, indirectOffset, 1, 0);

    // Finish recording commands to the render pass
    commandBuffer.endRenderPass();

    // Finish recording commands to the command buffer
    commandBuffer.end();
  }

  // Display the window
  displaySurface(ctx);
}

// Resize window
void resizeWindow(Context& ctx) {

  // Destroy framebuffers
  destroyFramebuffers(ctx);

  // Reset command buffer and record commands
  for(uint32_t i=0; i<ctx.numImages; i++) {
    ctx.commandBuffers[i].reset(vk::CommandBufferResetFlagBits::eReleaseResources);
  }

  recordCommands(ctx);
}

// Deallocate structures
void destroy(Context& ctx) {

  destroyBuffer(ctx, ctx.drawBuffer, ctx.drawBufferMemory);
  destroyPipeline(ctx);
  destroyFramebuffers(ctx);
  destroyRenderPass(ctx);
  destroySync(ctx);
  destroyCommandBuffers(ctx);
  destroySwapchain(ctx);
  destroySurface(ctx);
  DeviceUtils::destroyDevice(ctx);
}

