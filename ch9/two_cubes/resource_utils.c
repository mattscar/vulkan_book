#include "resource_utils.h"

/* Allocate memory based on requirements */
void allocate_memory(Context* ctx, VkMemoryRequirements mem_reqs, VkMemoryPropertyFlags type, VkDeviceMemory* device_mem) {

  uint32_t i, mem_index;
  VkPhysicalDeviceMemoryProperties mem_props;

  /* Obtain memory properties of the given device */
  vkGetPhysicalDeviceMemoryProperties(ctx->phys_device, &mem_props);

  /* Find a suitable memory type index */
  for(i = 0; i < mem_props.memoryTypeCount; i++) {
    if((mem_reqs.memoryTypeBits & (1 << i)) &&
      (mem_props.memoryTypes[i].propertyFlags & type)) {
      mem_index = i;
      break;
    }
  }

  /* Allocate memory */
  VkMemoryAllocateInfo mem_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = mem_reqs.size,
    .memoryTypeIndex = mem_index
  };
  ck(vkAllocateMemory(ctx->device, &mem_info, NULL, device_mem));
}

/* Initialize the resource's content */
void init_resource(VkDevice dev, VkMemoryPropertyFlags type, uint32_t size, void* data, VkDeviceMemory* device_mem) {

  /* Map a memory address to the resource */
  void *mem_ptr;
  ck(vkMapMemory(dev, *device_mem, 0, size, 0, &mem_ptr));

  /* Copy data to the memory address */
  memcpy(mem_ptr, data, size);

  /* For non-coherent memory, force write operation */
  if(!(type & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
    VkMappedMemoryRange map_range = {
      .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
      .pNext = NULL,
      .memory = *device_mem,
      .offset = 0,
      .size = VK_WHOLE_SIZE
    };
    ck(vkFlushMappedMemoryRanges(dev, 1, &map_range));
  }

  /* Remove the mapping */
  vkUnmapMemory(dev, *device_mem);
}

/* Create a buffer resource */
void create_buffer(Context* ctx, VkBufferUsageFlags usage, VkMemoryPropertyFlags type, 
  uint32_t size, void* data, VkDeviceMemory* device_mem, VkBuffer* buffer) {

  VkMemoryRequirements mem_reqs;

  /* Create the buffer object */
  const VkBufferCreateInfo buffer_info = {
    .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .size = size,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL
  };
  ck(vkCreateBuffer(ctx->device, &buffer_info, NULL, buffer));

  /* Determine memory requirements and allocate */
  vkGetBufferMemoryRequirements(ctx->device, *buffer, &mem_reqs);
  allocate_memory(ctx, mem_reqs, type, device_mem);

  /* Map the buffer to a memory location and copy data */
  if(data) {
    init_resource(ctx->device, type, size, data, device_mem);
  }

  /* Bind the buffer resource to memory */
  ck(vkBindBufferMemory(ctx->device, *buffer, *device_mem, 0));
}

/* Create an image resource */
void create_image(Context* ctx, VkImageUsageFlags usage, VkFormat format, 
  VkMemoryPropertyFlags type, uint32_t size, void* data, 
  VkDeviceMemory* device_mem, VkImage* image) {

  VkMemoryRequirements mem_reqs;

  /* Create the image object */
  VkImageCreateInfo image_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .imageType = VK_IMAGE_TYPE_2D,
    .format = format,
    .extent = { ctx->width, ctx->height, 1 },
    .mipLevels = 1,
    .arrayLayers = 1,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .tiling = VK_IMAGE_TILING_OPTIMAL,
    .usage = usage,
    .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    .queueFamilyIndexCount = 0,
    .pQueueFamilyIndices = NULL,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
  };
  ck(vkCreateImage(ctx->device, &image_info, NULL, image));

  /* Determine memory requirements and allocate */
  vkGetImageMemoryRequirements(ctx->device, *image, &mem_reqs);
  allocate_memory(ctx, mem_reqs, type, device_mem);

  /* Map the buffer to a memory location and copy data */
  if(data) {
    init_resource(ctx->device, type, size, data, device_mem);
  }

  /* Bind the buffer resource to memory */
  ck(vkBindImageMemory(ctx->device, *image, *device_mem, 0));
}

/* Transfer staging data */
void transfer_to_device_buffer(Context* ctx, void* data,
  size_t data_size, VkBuffer* device_buffer) {
  
  /* Structures needed for temp buffer */
  VkCommandBuffer temp_command_buffer;
  VkCommandBufferBeginInfo temp_begin_info;
  VkCommandPool temp_command_pool;
  VkDeviceMemory temp_memory;
  VkBuffer temp_buffer;  
  
  /* Create temp buffer */
  create_buffer(ctx, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
    data_size, data, &temp_memory, &temp_buffer);    

  /* Transfer data between buffers */
  temp_begin_info = 
    create_command_buffers(ctx, &temp_command_buffer,
      &temp_command_pool, 1, TRANSFER_QTYPE,
      VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);   

  /* Start recording commands */
  ck(vkBeginCommandBuffer(temp_command_buffer, &temp_begin_info));

  /* Copy data between buffers */
  VkBufferCopy buffer_copy = {
    .srcOffset = 0,
    .dstOffset = 0,
    .size = data_size
  };
  vkCmdCopyBuffer(temp_command_buffer, temp_buffer,
     *device_buffer, 1, &buffer_copy);

  /* Finish recording commands to the command buffer */
  ck(vkEndCommandBuffer(temp_command_buffer));

  /* Submit transfer command to queue */
  VkSubmitInfo temp_info = {
    .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
    .pNext = NULL,
    .waitSemaphoreCount = 0,
    .pWaitSemaphores = NULL,
    .pWaitDstStageMask = NULL,
    .commandBufferCount = 1,
    .pCommandBuffers = &temp_command_buffer,
    .signalSemaphoreCount = 0,
    .pSignalSemaphores = NULL
  };
  ck(vkQueueSubmit(ctx->queues[TRANSFER_QTYPE], 1, &temp_info, NULL));
  
  /* Wait for transfer to complete */
  ck(vkQueueWaitIdle(ctx->queues[TRANSFER_QTYPE]));

  /* Deallocate resources */
  vkFreeCommandBuffers(ctx->device, temp_command_pool, 1, &temp_command_buffer);  
  vkDestroyCommandPool(ctx->device, temp_command_pool, NULL); 
}

/* Destroy the buffer */
void destroy_buffer(Context* ctx, VkBuffer* buffer, VkDeviceMemory* memory) {

  if(*buffer) {
    vkDestroyBuffer(ctx->device, *buffer, NULL);
  }

  if(*memory) {
    vkFreeMemory(ctx->device, *memory, NULL);
  }
}

void destroy_image(Context* ctx, VkImage* image, VkDeviceMemory* memory) {
  
  if(*image) {
    vkDestroyImage(ctx->device, *image, NULL);
  }

  if(*memory) {
    vkFreeMemory(ctx->device, *memory, NULL);
  }  
}