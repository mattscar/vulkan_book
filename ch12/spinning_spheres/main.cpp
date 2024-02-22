#include "main.hpp"

// Create data for threads
glm::vec3 translateVectors[4] = {
  glm::vec3( 2.5,  2.5, -15.0),
  glm::vec3(-2.5,  2.5, -15.0),
  glm::vec3( 2.5, -2.5, -15.0),
  glm::vec3(-2.5, -2.5, -15.0)};

glm::vec4 colors[4] = {
  glm::vec4(1.0, 0.0, 0.0, 1.0),
  glm::vec4(0.0, 1.0, 0.0, 1.0),
  glm::vec4(0.0, 0.0, 1.0, 1.0),
  glm::vec4(1.0, 1.0, 0.0, 1.0)};
PushData pushData;

float rotationAngle = 0.0;

// Create the view/persepctive matrix
glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), 
  glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 vpMatrix = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 5.0f, 25.0f) * viewMatrix;

void createStructures(Context& ctx);

void recordSecondaryCommands(Context& ctx, uint32_t index) {

  // Set the viewport
  ctx.viewport.width = ctx.width * 1.0f;
  ctx.viewport.height = ctx.height * 1.0f;
  ctx.secCommandBuffers[index].setViewport(0, 1, &ctx.viewport);

  // Set the scissor dimensions
  ctx.scissor.extent.width = ctx.width * 1.0f;
  ctx.scissor.extent.height = ctx.height * 1.0f;
  ctx.secCommandBuffers[index].setScissor(0, 1, &ctx.scissor);

  // Start recording
  ck(ctx.secCommandBuffers[index].begin(ctx.secCommandBeginInfo));

  // Bind the vertex buffer to the command buffer
  ctx.secCommandBuffers[index].bindVertexBuffers(0, {ctx.drawBuffer}, {0});

  // Set push constant data
  glm::mat4 transMat = vpMatrix *
    glm::translate(glm::mat4(1.0f), translateVectors[index]);  
  pushData.mvpMatrix = glm::rotate(transMat, 
    glm::radians(rotationAngle), glm::vec3(0.0, 1.0, 0.0));
  pushData.color = colors[index];
  ctx.secCommandBuffers[index].pushConstants(ctx.pipelineLayout,
    vk::ShaderStageFlagBits::eVertex, 0, sizeof(pushData), &pushData);

  // Bind the graphics pipeline to the command buffer
  ctx.secCommandBuffers[index].bindPipeline(vk::PipelineBindPoint::eGraphics, ctx.pipeline);

  // Perform the draw operation
  ctx.secCommandBuffers[index].drawIndirect(ctx.drawBuffer, NUM_VERTICES * 3 * sizeof(float), 1, 0);

  // End recording
  ck(ctx.secCommandBuffers[index].end());
}

// Submit the command buffer and present the processed image
void draw(Context& ctx) {

  // Acquire the next image
  vk::ResultValue<uint32_t> res = ctx.device.acquireNextImageKHR(ctx.swapchain,
    UINT64_MAX, ctx.acquireCompleteSemaphore, nullptr);
  if(res.result == vk::Result::eErrorOutOfDateKHR) {
    return;
  }

  // Record commands to the primary command buffer
  ck(ctx.primCommandBuffers[0].begin(ctx.primCommandBeginInfo));

  // Record commands to the render pass
  ctx.renderPassBeginInfo.renderArea.extent.width = ctx.width;
  ctx.renderPassBeginInfo.renderArea.extent.height = ctx.height;
  ctx.renderPassBeginInfo.framebuffer = ctx.framebuffers[res.value];
  ctx.primCommandBuffers[0].beginRenderPass(ctx.renderPassBeginInfo,
    vk::SubpassContents::eInline);

  // Create inheritance info
  vk::CommandBufferInheritanceInfo inheritanceInfo =
    vk::CommandBufferInheritanceInfo()
    .setRenderPass(ctx.renderPass)
    .setFramebuffer(ctx.framebuffers[res.value]);
  ctx.secCommandBeginInfo.pInheritanceInfo = &inheritanceInfo;

  // Record commands to the secondary command buffers
  for(uint32_t i=0; i<ctx.drawThreads.size(); ++i) {
    ctx.drawThreads[i] = std::thread(recordSecondaryCommands, std::ref(ctx), i);
  }
  for(uint32_t i=0; i<ctx.drawThreads.size(); ++i) {
    ctx.drawThreads[i].join();
  }

  // Record commands from secondary command buffers
  ctx.primCommandBuffers[0].executeCommands(ctx.secCommandBuffers);

  // Finish recording commands to the render pass
  ctx.primCommandBuffers[0].endRenderPass();

  // Finish recording commands to the command buffer
  ck(ctx.primCommandBuffers[0].end());

  // Submit the command buffer to the queue
  uint32_t index = res.value;
  ck(ctx.queues[GRAPHICS_QTYPE].submit(ctx.submitInfo, ctx.fence));

  // Increment rotation angle
  rotationAngle += 0.5;
  
  // Wait for the command buffer to be executed
	ctx.device.waitForFences({ctx.fence}, VK_TRUE, 100000000);
  ctx.device.resetFences({ctx.fence});

  // Present the image
  ctx.presentInfo.pImageIndices = &index;
  ck(ctx.queues[GRAPHICS_QTYPE].presentKHR(ctx.presentInfo));
}

int main() {
  Context ctx;
  ctx.x = 0; ctx.y = 0;
  ctx.width = 400; ctx.height = 400;
  DrawData drawData;
  float phi, theta;
  uint32_t i;

  // One thread for each sphere
  ctx.drawThreads.resize(NUM_SPHERES);

  // Set the sphere's coordinates
  for(int32_t lon=-8; lon<8; ++lon) {
    for(int32_t lat=0; lat<16; ++lat) {

      theta = M_PI * lon/8.0;
      phi = M_PI * lat/15.0;

      i = (lon + 8) * 16 + lat;
      drawData.vertices[i].x = sin(phi) * sin(theta);
      drawData.vertices[i].y = cos(phi);
      drawData.vertices[i].z = sin(phi) * cos(theta);
    }
  }

  // Initialize drawing data
  drawData.indirectData = vk::DrawIndirectCommand()
    .setVertexCount(NUM_VERTICES)
    .setInstanceCount(1)
    .setFirstVertex(0)
    .setFirstInstance(0);

  // Clear values for the framebuffers
  vk::ClearColorValue color(std::array<float, 4>{1.0f, 1.0f, 1.0f, 0.25f});
  std::vector<vk::ClearValue> clearValues;
  clearValues.push_back(vk::ClearValue().setColor(color));

  // Set dynamic viewport
  ctx.viewport = vk::Viewport()
    .setX(1.0f * ctx.x)
    .setY(1.0f * ctx.y)
    .setWidth(1.0f * ctx.width)
    .setHeight(1.0f * ctx.height)
    .setMinDepth(0.0f)
    .setMaxDepth(1.0f);

  // Define a scissor rectangle
  ctx.scissor = vk::Rect2D()
    .setOffset(vk::Offset2D(ctx.x, ctx.y))
    .setExtent(vk::Extent2D(ctx.width, ctx.height));

  // Create data structures
  std::vector<vk::QueueFlagBits> flags =
    {vk::QueueFlagBits::eGraphics, vk::QueueFlagBits::eTransfer};
  device_utils::createDevice(ctx, flags, std::string("Spinning Spheres"));
  surface_utils::createSurface(ctx);
  sync_utils::createSync(ctx);
  renderpass_utils::createRenderPass(ctx, clearValues);

  // Create device buffer
  resource_utils::createBuffer(ctx,
    vk::BufferUsageFlagBits::eVertexBuffer
    | vk::BufferUsageFlagBits::eIndexBuffer
    | vk::BufferUsageFlagBits::eIndirectBuffer
    | vk::BufferUsageFlagBits::eTransferDst,
    vk::MemoryPropertyFlagBits::eDeviceLocal,
    sizeof(drawData), nullptr,
    ctx.drawBufferMemory, ctx.drawBuffer);

  // Transfer data from staging buffer to device buffer
  resource_utils::transferToDeviceBuffer(ctx,
    &drawData, sizeof(drawData), ctx.drawBuffer);

  // Create pipeline
  pipeline_utils::createPipeline(ctx);

  // Start recording commands
  createStructures(ctx);

  surface_utils::displaySurface(ctx);
  return 0;
}

void createStructures(Context& ctx) {

  vk::PipelineStageFlags stage = vk::PipelineStageFlagBits::eTransfer;

  // Create structures
  swapchain_utils::createSwapchain(ctx);

  // Create command buffers
  if(ctx.primCommandBuffers.empty()) {

    // Create primary command buffer
    ctx.primCommandBeginInfo =
      command_utils::createCommandBuffers(ctx, ctx.primCommandBuffers,
        ctx.commandPool, 1, GRAPHICS_QTYPE,
        vk::CommandBufferUsageFlagBits::eSimultaneousUse);

    // Create secondary command buffers
    ctx.secCommandBeginInfo =
      command_utils::createCommandBuffers(ctx, ctx.secCommandBuffers,
        ctx.commandPool, NUM_SPHERES, GRAPHICS_QTYPE,
        vk::CommandBufferUsageFlagBits::eRenderPassContinue,
        vk::CommandBufferLevel::eSecondary);

  }
  framebuffer_utils::createFramebuffers(ctx);
  sync_utils::createDraw(ctx, stage);
  ctx.submitInfo.pCommandBuffers = &ctx.primCommandBuffers[0];
}

// Resize window
void resizeWindow(Context& ctx) {

  // Destroy framebuffers
  framebuffer_utils::destroyFramebuffers(ctx);

  // Reset command buffer
  ck(ctx.primCommandBuffers[0].reset(vk::CommandBufferResetFlagBits::eReleaseResources));

  createStructures(ctx);
  surface_utils::displaySurface(ctx);
}

// Deallocate structures
void destroy(Context& ctx) {

  resource_utils::destroyBuffer(ctx, ctx.drawBuffer, ctx.drawBufferMemory);
  pipeline_utils::destroyPipeline(ctx);
  framebuffer_utils::destroyFramebuffers(ctx);
  renderpass_utils::destroyRenderPass(ctx);
  sync_utils::destroySync(ctx);
  command_utils::destroyCommandBuffers(ctx.device,
    ctx.primCommandBuffers, ctx.commandPool);
  swapchain_utils::destroySwapchain(ctx);
  surface_utils::destroySurface(ctx);
  device_utils::destroyDevice(ctx);
}

