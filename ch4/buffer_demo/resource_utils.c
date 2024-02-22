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
void init_resource(VkDevice dev, VkMemoryPropertyFlags type, uint32_t size, uint32_t* data, VkDeviceMemory* device_mem) {

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

/* Create the buffer with the given size */
void create_buffer(Context* ctx, VkBufferUsageFlags usage, VkMemoryPropertyFlags type, 
  uint32_t size, void* data, VkDeviceMemory* device_mem, VkBuffer* buffer) {

  /* Create the resource buffer */
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
  VkMemoryRequirements mem_reqs;
  vkGetBufferMemoryRequirements(ctx->device, *buffer, &mem_reqs);
  allocate_memory(ctx, mem_reqs, type, device_mem);

  /* Map the buffer to a memory location and copy data */
  if(data) {
    init_resource(ctx->device, type, size, data, device_mem);
  }

  /* Bind the buffer resource to memory */
  ck(vkBindBufferMemory(ctx->device, *buffer, *device_mem, 0));
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