#include "gli_utils.h"

VkFormat convert_format(gli::format gli_format) {

  VkFormat format;

  switch(gli_format) {

    case gli::FORMAT_RG4_UNORM_PACK8:
		  format = VK_FORMAT_R4G4_UNORM_PACK8;
      break;

      format = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
      break;

      format = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
      break;

      format = VK_FORMAT_R5G6B5_UNORM_PACK16;
      break;

      format = VK_FORMAT_B5G6R5_UNORM_PACK16;
      break;

      format = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
      break;

      format = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
      break;

      format = VK_FORMAT_A1R5G5B5_UNORM_PACK16;
      break;

      format = VK_FORMAT_R8_UNORM;
      break;

      format = VK_FORMAT_R8_SNORM;
      break;

      format = VK_FORMAT_R8_USCALED;
      break;

      format = VK_FORMAT_R8_SSCALED;
      break;

      format = VK_FORMAT_R8_UINT;
      break;

      format = VK_FORMAT_R8_SINT;
      break;

      format = VK_FORMAT_R8_SRGB;
      break;

      format = VK_FORMAT_R8G8_UNORM;
      break;

      format = VK_FORMAT_R8G8_SNORM;
      break;

      format = VK_FORMAT_R8G8_USCALED;
      break;

      format = VK_FORMAT_R8G8_SSCALED;
      break;

      format = VK_FORMAT_R8G8_UINT;
      break;

      format = VK_FORMAT_R8G8_SINT;
      break;

      format = VK_FORMAT_R8G8_SRGB;
      break;

      format = VK_FORMAT_R8G8B8_UNORM;
      break;

      format = VK_FORMAT_R8G8B8_SNORM;
      break;

      format = VK_FORMAT_R8G8B8_USCALED;
      break;

      format = VK_FORMAT_R8G8B8_SSCALED;
      break;

      format = VK_FORMAT_R8G8B8_UINT;
      break;

      format = VK_FORMAT_R8G8B8_SINT;
      break;

      format = VK_FORMAT_R8G8B8_SRGB;
      break;

      format = VK_FORMAT_B8G8R8_UNORM;
      break;

      format = VK_FORMAT_B8G8R8_SNORM;
      break;

      format = VK_FORMAT_B8G8R8_USCALED;
      break;

      format = VK_FORMAT_B8G8R8_SSCALED;
      break;

      format = VK_FORMAT_B8G8R8_UINT;
      break;

      format = VK_FORMAT_B8G8R8_SINT;
      break;

      format = VK_FORMAT_B8G8R8_SRGB;
      break;

      format = VK_FORMAT_R8G8B8A8_UNORM;
      break;

      format = VK_FORMAT_R8G8B8A8_SNORM;
      break;

      format = VK_FORMAT_R8G8B8A8_USCALED;
      break;

      format = VK_FORMAT_R8G8B8A8_SSCALED;
      break;

      format = VK_FORMAT_R8G8B8A8_UINT;
      break;

      format = VK_FORMAT_R8G8B8A8_SINT;
      break;

      format = VK_FORMAT_R8G8B8A8_SRGB;
      break;

      format = VK_FORMAT_B8G8R8A8_UNORM;
      break;

      format = VK_FORMAT_B8G8R8A8_SNORM;
      break;

      format = VK_FORMAT_B8G8R8A8_USCALED;
      break;

      format = VK_FORMAT_B8G8R8A8_SSCALED;
      break;

      format = VK_FORMAT_B8G8R8A8_UINT;
      break;

      format = VK_FORMAT_B8G8R8A8_SINT;
      break;

      format = VK_FORMAT_B8G8R8A8_SRGB;
      break;

      format = VK_FORMAT_R8G8B8A8_UNORM;
      break;

      format = VK_FORMAT_R8G8B8A8_SNORM;
      break;

      format = VK_FORMAT_R8G8B8A8_USCALED;
      break;

      format = VK_FORMAT_R8G8B8A8_SSCALED;
      break;

      format = VK_FORMAT_R8G8B8A8_UINT;
      break;

      format = VK_FORMAT_R8G8B8A8_SINT;
      break;

      format = VK_FORMAT_R8G8B8A8_SRGB;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_R16_UNORM;
      break;

      format = VK_FORMAT_R16_SNORM;
      break;

      format = VK_FORMAT_R16_USCALED;
      break;

      format = VK_FORMAT_R16_SSCALED;
      break;

      format = VK_FORMAT_R16_UINT;
      break;

      format = VK_FORMAT_R16_SINT;
      break;

      format = VK_FORMAT_R16_SFLOAT;
      break;

      format = VK_FORMAT_R16G16_UNORM;
      break;

      format = VK_FORMAT_R16G16_SNORM;
      break;

      format = VK_FORMAT_R16G16_USCALED;
      break;

      format = VK_FORMAT_R16G16_SSCALED;
      break;

      format = VK_FORMAT_R16G16_UINT;
      break;

      format = VK_FORMAT_R16G16_SINT;
      break;

      format = VK_FORMAT_R16G16_SFLOAT;
      break;

      format = VK_FORMAT_R16G16B16_UNORM;
      break;

      format = VK_FORMAT_R16G16B16_SNORM;
      break;

      format = VK_FORMAT_R16G16B16_USCALED;
      break;

      format = VK_FORMAT_R16G16B16_SSCALED;
      break;

      format = VK_FORMAT_R16G16B16_UINT;
      break;

      format = VK_FORMAT_R16G16B16_SINT;
      break;

      format = VK_FORMAT_R16G16B16_SFLOAT;
      break;

      format = VK_FORMAT_R16G16B16A16_UNORM;
      break;

      format = VK_FORMAT_R16G16B16A16_SNORM;
      break;

      format = VK_FORMAT_R16G16B16A16_USCALED;
      break;

      format = VK_FORMAT_R16G16B16A16_SSCALED;
      break;

      format = VK_FORMAT_R16G16B16A16_UINT;
      break;

      format = VK_FORMAT_R16G16B16A16_SINT;
      break;

      format = VK_FORMAT_R16G16B16A16_SFLOAT;
      break;

      format = VK_FORMAT_R32_UINT;
      break;

      format = VK_FORMAT_R32_SINT;
      break;

      format = VK_FORMAT_R32_SFLOAT;
      break;

      format = VK_FORMAT_R32G32_UINT;
      break;

      format = VK_FORMAT_R32G32_SINT;
      break;

      format = VK_FORMAT_R32G32_SFLOAT;
      break;

      format = VK_FORMAT_R32G32B32_UINT;
      break;

      format = VK_FORMAT_R32G32B32_SINT;
      break;

      format = VK_FORMAT_R32G32B32_SFLOAT;
      break;

      format = VK_FORMAT_R32G32B32A32_UINT;
      break;

      format = VK_FORMAT_R32G32B32A32_SINT;
      break;

      format = VK_FORMAT_R32G32B32A32_SFLOAT;
      break;

      format = VK_FORMAT_R64_UINT;
      break;

      format = VK_FORMAT_R64_SINT;
      break;

      format = VK_FORMAT_R64_SFLOAT;
      break;

      format = VK_FORMAT_R64G64_UINT;
      break;

      format = VK_FORMAT_R64G64_SINT;
      break;

      format = VK_FORMAT_R64G64_SFLOAT;
      break;

      format = VK_FORMAT_R64G64B64_UINT;
      break;

      format = VK_FORMAT_R64G64B64_SINT;
      break;

      format = VK_FORMAT_R64G64B64_SFLOAT;
      break;

      format = VK_FORMAT_R64G64B64A64_UINT;
      break;

      format = VK_FORMAT_R64G64B64A64_SINT;
      break;

      format = VK_FORMAT_R64G64B64A64_SFLOAT;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_D16_UNORM;
      break;

      format = VK_FORMAT_X8_D24_UNORM_PACK32;
      break;

      format = VK_FORMAT_D32_SFLOAT;
      break;

      format = VK_FORMAT_S8_UINT;
      break;

      format = VK_FORMAT_D16_UNORM_S8_UINT;
      break;

      format = VK_FORMAT_D24_UNORM_S8_UINT;
      break;

      format = VK_FORMAT_D32_SFLOAT_S8_UINT;
      break;

      format = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
      break;

      format = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
      break;

      format = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
      break;

      format = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
      break;

      format = VK_FORMAT_BC2_UNORM_BLOCK;
      break;

      format = VK_FORMAT_BC2_SRGB_BLOCK;
      break;

      format = VK_FORMAT_BC3_UNORM_BLOCK;
      break;

      format = VK_FORMAT_BC3_SRGB_BLOCK;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
      break;

      format = VK_FORMAT_EAC_R11_UNORM_BLOCK;
      break;

      format = VK_FORMAT_EAC_R11_SNORM_BLOCK;
      break;

      format = VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
      break;

      format = VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
      break;

      format = VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
      break;

      format = VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

    case gli::FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

      format = VK_FORMAT_R8_UNORM;
      break;

      format = VK_FORMAT_R8_UNORM;
      break;

      format = VK_FORMAT_R8G8_UNORM;
      break;

      format = VK_FORMAT_R16_UNORM;
      break;

      format = VK_FORMAT_R16_UNORM;
      break;

      format = VK_FORMAT_R16G16_UNORM;
      break;

      format = VK_FORMAT_B8G8R8_UNORM;
      break;

      format = VK_FORMAT_B8G8R8_SRGB;
      break;

      format = VK_FORMAT_UNDEFINED;
      break;

    default:
      format = VK_FORMAT_UNDEFINED;
      break;
  }

  if(format == VK_FORMAT_UNDEFINED) {
    printf("Format not recognized by Vulkan");
  }
  return format;
}