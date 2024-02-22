#include "ResourceUtils.hpp"

// Allocate memory based on requirements
void allocateMemory(Context& ctx, vk::MemoryRequirements memReqs,
  vk::MemoryPropertyFlags type, vk::DeviceMemory& deviceMem) {

  uint32_t i, memIndex;

  // Obtain memory properties of the given device
  vk::PhysicalDeviceMemoryProperties memProps =
    ctx.physDevice.getMemoryProperties();

  // Find a suitable memory type index
  for(i=0; i < memProps.memoryTypeCount; ++i) {
    if((memReqs.memoryTypeBits & (1 << i)) &&
      (memProps.memoryTypes[i].propertyFlags & type)) {
      memIndex = i;
      break;
    }
  }

  // Allocate memory
  vk::MemoryAllocateInfo memInfo = vk::MemoryAllocateInfo()
    .setAllocationSize(memReqs.size)
    .setMemoryTypeIndex(memIndex);

  deviceMem = ctx.device.allocateMemory(memInfo);
}

// Initialize the resource's content
void initResource(vk::Device dev, vk::MemoryPropertyFlags type,
  uint32_t size, void* data, vk::DeviceMemory& deviceMem) {

  // Map a memory address to the resource
  void *memPtr = dev.mapMemory(deviceMem, 0, size);

  // Copy data to the memory address
  std::memcpy(memPtr, data, size);

  // For non-coherent memory, force write operation
  if(!(type & vk::MemoryPropertyFlagBits::eHostCoherent)) {
    vk::MappedMemoryRange mapRange = vk::MappedMemoryRange()
      .setMemory(deviceMem)
      .setOffset(0)
      .setSize(VK_WHOLE_SIZE);
    dev.flushMappedMemoryRanges(1, &mapRange);
  }

  // Remove the mapping
  dev.unmapMemory(deviceMem);
}

// Create the buffer with the given size
void createBuffer(Context& ctx, vk::BufferUsageFlags usage,
  vk::MemoryPropertyFlags type, uint32_t size, void* data,
  vk::DeviceMemory& deviceMem, vk::Buffer& buffer) {

  vk::MemoryRequirements memReqs;

  // Create the resource buffer
  vk::BufferCreateInfo bufferInfo = vk::BufferCreateInfo()
    .setSize(size)
    .setUsage(usage)
    .setSharingMode(vk::SharingMode::eExclusive)
    .setQueueFamilyIndexCount(0)
    .setPQueueFamilyIndices(nullptr);
  buffer = ctx.device.createBuffer(bufferInfo);

  // Determine memory requirements and allocate
  memReqs = ctx.device.getBufferMemoryRequirements(buffer);
  allocateMemory(ctx, memReqs, type, deviceMem);

  // Map the buffer to a memory location and copy data
  if(data) {
    initResource(ctx.device, type, size, data, deviceMem);
  }

  // Bind the buffer resource to memory
  ctx.device.bindBufferMemory(buffer, deviceMem, 0);
}

// Transfer staging data
void transferToDeviceBuffer(Context& ctx, void* data,
  size_t dataSize, vk::Buffer& deviceBuffer) {

  // Structures needed for temp buffer
  std::vector<vk::CommandBuffer> tempCommandBuffers;
  vk::CommandBufferBeginInfo tempBeginInfo;
  vk::CommandPool tempCommandPool;
  vk::DeviceMemory tempMemory;
  vk::Buffer tempBuffer;

  // Create temp buffer
  createBuffer(ctx, vk::BufferUsageFlagBits::eTransferSrc,
    vk::MemoryPropertyFlagBits::eHostVisible |
    vk::MemoryPropertyFlagBits::eHostCoherent,
    dataSize, data, tempMemory, tempBuffer);

  // Transfer data between buffers
  tempBeginInfo =
    createCommandBuffers(ctx, tempCommandBuffers,
      tempCommandPool, 1, TRANSFER_QTYPE,
      vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
      
  // Start recording commands
  tempCommandBuffers[0].begin(tempBeginInfo);

  // Copy data between buffers
  vk::BufferCopy bufferCopy = vk::BufferCopy()
    .setSrcOffset(0)
    .setDstOffset(0)
    .setSize(dataSize);

  tempCommandBuffers[0].copyBuffer(tempBuffer,
    deviceBuffer, 1, &bufferCopy);

  // Finish recording commands to the command buffer
  tempCommandBuffers[0].end();

  // Submit transfer command to queue
  vk::SubmitInfo tempInfo = vk::SubmitInfo()
    .setWaitSemaphoreCount(0)
    .setPWaitSemaphores(nullptr)
    .setPWaitDstStageMask(nullptr)
    .setCommandBufferCount(1)
    .setPCommandBuffers(tempCommandBuffers.data())
    .setSignalSemaphoreCount(0)
    .setPSignalSemaphores(nullptr);
  ctx.queues[TRANSFER_QTYPE].submit(tempInfo, nullptr);

  // Wait for transfer to complete
  ctx.queues[TRANSFER_QTYPE].waitIdle();

  // Deallocate resources
  ctx.device.freeCommandBuffers(tempCommandPool, tempCommandBuffers);
  ctx.device.destroyCommandPool(tempCommandPool);
}

// Create an image view 
void createImageView(Context& ctx, vk::Format format,
  vk::ImageAspectFlags flags, vk::Image& image,
  vk::ImageView& imageView) {

  vk::ImageViewCreateInfo imageViewInfo = vk::ImageViewCreateInfo()
    .setImage(image)
    .setViewType(vk::ImageViewType::e2D)
    .setFormat(format)
    .setSubresourceRange({
      .aspectMask = flags,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }); 
  imageView = ctx.device.createImageView(imageViewInfo);
}

// Destroy the buffer
void destroyBuffer(Context& ctx, vk::Buffer& buffer, vk::DeviceMemory& memory) {

  if(buffer) {
    ctx.device.destroyBuffer(buffer);
  }

  if(memory) {
    ctx.device.freeMemory(memory);
  }
}