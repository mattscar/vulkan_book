#include "resource_utils.h"

/* Allocate memory based on requirements */
VkBool32 allocate_memory(Context* ctx, VkMemoryRequirements mem_reqs, VkDeviceMemory* device_mem) {

  VkBool32 coherent = VK_FALSE;
  uint32_t i, mem_index;
  VkPhysicalDeviceMemoryProperties mem_props;

  /* Obtain memory properties of the given device */
  vkGetPhysicalDeviceMemoryProperties(ctx->phys_device, &mem_props);

  /* Find a suitable memory type index */
  for(i = 0; i < mem_props.memoryTypeCount; i++) {
    if((mem_reqs.memoryTypeBits & (1 << i)) &&
      (mem_props.memoryTypes[i].propertyFlags &
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)) {
      mem_index = i;

      /* Check for coherent memory */
      if(mem_props.memoryTypes[i].propertyFlags &
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
        coherent = VK_TRUE;
      }
      break;
    }
  }

  /* Allocate memory */
  const VkMemoryAllocateInfo mem_info = {
    .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
    .pNext = NULL,
    .allocationSize = mem_reqs.size,
    .memoryTypeIndex = mem_index
  };
  ck(vkAllocateMemory(ctx->device, &mem_info, NULL, device_mem));

  return coherent;
}

/* Initialize the resource's content */
void init_resource(VkDevice dev, VkBool32 coherent, uint32_t size, 
  uint32_t* data, VkDeviceMemory* device_mem) {

  /* Map a memory address to the resource */
  void *mem_ptr;
  ck(vkMapMemory(dev, *device_mem, 0, size, 0, &mem_ptr));

  /* Copy data to the memory address */
  memcpy(mem_ptr, data, size);

  /* If memory non-coherent, force write operation */
  if(coherent == VK_FALSE) {
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
void create_buffer(Context* ctx, VkBufferUsageFlags usage, uint32_t size, 
  void* data, VkDeviceMemory* device_mem, VkBuffer* buffer) {

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
  vkGetBufferMemoryRequirements(ctx->device, ctx->vertex_buffer, &mem_reqs);
  VkBool32 coherent = allocate_memory(ctx, mem_reqs, device_mem);

  /* Map the buffer to a memory location and copy data */
  if(data) {
    init_resource(ctx->device, coherent, size, data, device_mem);
  }

  /* Bind the buffer resource to memory */
  ck(vkBindBufferMemory(ctx->device, *buffer, *device_mem, 0));
}

/* Create an image view */
void create_image_view(Context* ctx, VkFormat format, 
  VkImageAspectFlags flags, VkImage* image, 
  VkImageView *image_view) {

  VkImageViewCreateInfo image_view_info = {
    .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .image = *image,
    .viewType = VK_IMAGE_VIEW_TYPE_2D,
    .format = format,
    .components = {
      .r = VK_COMPONENT_SWIZZLE_IDENTITY,
      .g = VK_COMPONENT_SWIZZLE_IDENTITY,
      .b = VK_COMPONENT_SWIZZLE_IDENTITY,
      .a = VK_COMPONENT_SWIZZLE_IDENTITY
    },
    .subresourceRange = {
      .aspectMask = flags,
      .baseMipLevel = 0,
      .levelCount = 1,
      .baseArrayLayer = 0,
      .layerCount = 1
    }
  };
  ck(vkCreateImageView(ctx->device, &image_view_info,
    NULL, image_view));
}

/* Destroy the buffer */
void destroy_buffer(Context* ctx, VkDeviceMemory* device_mem, VkBuffer* buffer) {

  if(*buffer) {
    vkDestroyBuffer(ctx->device, *buffer, NULL);
  }

  if(*device_mem) {
    vkFreeMemory(ctx->device, *device_mem, NULL);
  }
}