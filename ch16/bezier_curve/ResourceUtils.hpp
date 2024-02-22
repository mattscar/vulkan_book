#ifndef RESOURCE_UTILS_H_
#define RESOURCE_UTILS_H_

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"
#include "ErrorUtils.hpp"
#include "CommandUtils.hpp"

void createBuffer(Context& ctx, vk::BufferUsageFlags usage, 
  vk::MemoryPropertyFlags type, uint32_t size, void* data, 
  vk::DeviceMemory& deviceMem, vk::Buffer& buffer);

void transferToDeviceBuffer(Context& ctx, void* data,
  size_t dataSize, vk::Buffer& deviceBuffer);

void createImageView(Context& ctx, vk::Format format, 
  vk::ImageAspectFlags flags, vk::Image& image, 
  vk::ImageView& imageView);  
  
void destroyBuffer(Context& ctx, vk::Buffer& buffer, vk::DeviceMemory& memory);

#endif // RESOURCE_UTILS_H_ 