#include "gli_utils.h"

VkFormat convert_format(gli::format gli_format) {

  VkFormat format;

  switch(gli_format) {

    case gli::FORMAT_RG4_UNORM_PACK8:
		  format = VK_FORMAT_R4G4_UNORM_PACK8;
      break;
    case gli::FORMAT_RGBA4_UNORM_PACK16:
      format = VK_FORMAT_R4G4B4A4_UNORM_PACK16;
      break;
    case gli::FORMAT_BGRA4_UNORM_PACK16:
      format = VK_FORMAT_B4G4R4A4_UNORM_PACK16;
      break;
    case gli::FORMAT_R5G6B5_UNORM_PACK16:
      format = VK_FORMAT_R5G6B5_UNORM_PACK16;
      break;
    case gli::FORMAT_B5G6R5_UNORM_PACK16:
      format = VK_FORMAT_B5G6R5_UNORM_PACK16;
      break;
    case gli::FORMAT_RGB5A1_UNORM_PACK16:
      format = VK_FORMAT_R5G5B5A1_UNORM_PACK16;
      break;
    case gli::FORMAT_BGR5A1_UNORM_PACK16:
      format = VK_FORMAT_B5G5R5A1_UNORM_PACK16;
      break;
    case gli::FORMAT_A1RGB5_UNORM_PACK16:
      format = VK_FORMAT_A1R5G5B5_UNORM_PACK16;
      break;
    case gli::FORMAT_R8_UNORM_PACK8:
      format = VK_FORMAT_R8_UNORM;
      break;
    case gli::FORMAT_R8_SNORM_PACK8:
      format = VK_FORMAT_R8_SNORM;
      break;
    case gli::FORMAT_R8_USCALED_PACK8:
      format = VK_FORMAT_R8_USCALED;
      break;
    case gli::FORMAT_R8_SSCALED_PACK8:
      format = VK_FORMAT_R8_SSCALED;
      break;
    case gli::FORMAT_R8_UINT_PACK8:
      format = VK_FORMAT_R8_UINT;
      break;
    case gli::FORMAT_R8_SINT_PACK8:
      format = VK_FORMAT_R8_SINT;
      break;
    case gli::FORMAT_R8_SRGB_PACK8:
      format = VK_FORMAT_R8_SRGB;
      break;
    case gli::FORMAT_RG8_UNORM_PACK8:
      format = VK_FORMAT_R8G8_UNORM;
      break;
    case gli::FORMAT_RG8_SNORM_PACK8:
      format = VK_FORMAT_R8G8_SNORM;
      break;
    case gli::FORMAT_RG8_USCALED_PACK8:
      format = VK_FORMAT_R8G8_USCALED;
      break;
    case gli::FORMAT_RG8_SSCALED_PACK8:
      format = VK_FORMAT_R8G8_SSCALED;
      break;
    case gli::FORMAT_RG8_UINT_PACK8:
      format = VK_FORMAT_R8G8_UINT;
      break;
    case gli::FORMAT_RG8_SINT_PACK8:
      format = VK_FORMAT_R8G8_SINT;
      break;
    case gli::FORMAT_RG8_SRGB_PACK8:
      format = VK_FORMAT_R8G8_SRGB;
      break;
    case gli::FORMAT_RGB8_UNORM_PACK8:
      format = VK_FORMAT_R8G8B8_UNORM;
      break;
    case gli::FORMAT_RGB8_SNORM_PACK8:
      format = VK_FORMAT_R8G8B8_SNORM;
      break;
    case gli::FORMAT_RGB8_USCALED_PACK8:
      format = VK_FORMAT_R8G8B8_USCALED;
      break;
    case gli::FORMAT_RGB8_SSCALED_PACK8:
      format = VK_FORMAT_R8G8B8_SSCALED;
      break;
    case gli::FORMAT_RGB8_UINT_PACK8:
      format = VK_FORMAT_R8G8B8_UINT;
      break;
    case gli::FORMAT_RGB8_SINT_PACK8:
      format = VK_FORMAT_R8G8B8_SINT;
      break;
    case gli::FORMAT_RGB8_SRGB_PACK8:
      format = VK_FORMAT_R8G8B8_SRGB;
      break;
    case gli::FORMAT_BGR8_UNORM_PACK8:
      format = VK_FORMAT_B8G8R8_UNORM;
      break;
    case gli::FORMAT_BGR8_SNORM_PACK8:
      format = VK_FORMAT_B8G8R8_SNORM;
      break;
    case gli::FORMAT_BGR8_USCALED_PACK8:
      format = VK_FORMAT_B8G8R8_USCALED;
      break;
    case gli::FORMAT_BGR8_SSCALED_PACK8:
      format = VK_FORMAT_B8G8R8_SSCALED;
      break;
    case gli::FORMAT_BGR8_UINT_PACK8:
      format = VK_FORMAT_B8G8R8_UINT;
      break;
    case gli::FORMAT_BGR8_SINT_PACK8:
      format = VK_FORMAT_B8G8R8_SINT;
      break;
    case gli::FORMAT_BGR8_SRGB_PACK8:
      format = VK_FORMAT_B8G8R8_SRGB;
      break;
    case gli::FORMAT_RGBA8_UNORM_PACK8:
      format = VK_FORMAT_R8G8B8A8_UNORM;
      break;
    case gli::FORMAT_RGBA8_SNORM_PACK8:
      format = VK_FORMAT_R8G8B8A8_SNORM;
      break;
    case gli::FORMAT_RGBA8_USCALED_PACK8:
      format = VK_FORMAT_R8G8B8A8_USCALED;
      break;
    case gli::FORMAT_RGBA8_SSCALED_PACK8:
      format = VK_FORMAT_R8G8B8A8_SSCALED;
      break;
    case gli::FORMAT_RGBA8_UINT_PACK8:
      format = VK_FORMAT_R8G8B8A8_UINT;
      break;
    case gli::FORMAT_RGBA8_SINT_PACK8:
      format = VK_FORMAT_R8G8B8A8_SINT;
      break;
    case gli::FORMAT_RGBA8_SRGB_PACK8:
      format = VK_FORMAT_R8G8B8A8_SRGB;
      break;
    case gli::FORMAT_BGRA8_UNORM_PACK8:
      format = VK_FORMAT_B8G8R8A8_UNORM;
      break;
    case gli::FORMAT_BGRA8_SNORM_PACK8:
      format = VK_FORMAT_B8G8R8A8_SNORM;
      break;
    case gli::FORMAT_BGRA8_USCALED_PACK8:
      format = VK_FORMAT_B8G8R8A8_USCALED;
      break;
    case gli::FORMAT_BGRA8_SSCALED_PACK8:
      format = VK_FORMAT_B8G8R8A8_SSCALED;
      break;
    case gli::FORMAT_BGRA8_UINT_PACK8:
      format = VK_FORMAT_B8G8R8A8_UINT;
      break;
    case gli::FORMAT_BGRA8_SINT_PACK8:
      format = VK_FORMAT_B8G8R8A8_SINT;
      break;
    case gli::FORMAT_BGRA8_SRGB_PACK8:
      format = VK_FORMAT_B8G8R8A8_SRGB;
      break;
    case gli::FORMAT_RGBA8_UNORM_PACK32:
      format = VK_FORMAT_R8G8B8A8_UNORM;
      break;
    case gli::FORMAT_RGBA8_SNORM_PACK32:
      format = VK_FORMAT_R8G8B8A8_SNORM;
      break;
    case gli::FORMAT_RGBA8_USCALED_PACK32:
      format = VK_FORMAT_R8G8B8A8_USCALED;
      break;
    case gli::FORMAT_RGBA8_SSCALED_PACK32:
      format = VK_FORMAT_R8G8B8A8_SSCALED;
      break;
    case gli::FORMAT_RGBA8_UINT_PACK32:
      format = VK_FORMAT_R8G8B8A8_UINT;
      break;
    case gli::FORMAT_RGBA8_SINT_PACK32:
      format = VK_FORMAT_R8G8B8A8_SINT;
      break;
    case gli::FORMAT_RGBA8_SRGB_PACK32:
      format = VK_FORMAT_R8G8B8A8_SRGB;
      break;
    case gli::FORMAT_RGB10A2_UNORM_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB10A2_SNORM_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB10A2_USCALED_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB10A2_SSCALED_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB10A2_UINT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB10A2_SINT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_UNORM_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_SNORM_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_USCALED_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_SSCALED_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_UINT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_BGR10A2_SINT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_R16_UNORM_PACK16:
      format = VK_FORMAT_R16_UNORM;
      break;
    case gli::FORMAT_R16_SNORM_PACK16:
      format = VK_FORMAT_R16_SNORM;
      break;
    case gli::FORMAT_R16_USCALED_PACK16:
      format = VK_FORMAT_R16_USCALED;
      break;
    case gli::FORMAT_R16_SSCALED_PACK16:
      format = VK_FORMAT_R16_SSCALED;
      break;
    case gli::FORMAT_R16_UINT_PACK16:
      format = VK_FORMAT_R16_UINT;
      break;
    case gli::FORMAT_R16_SINT_PACK16:
      format = VK_FORMAT_R16_SINT;
      break;
    case gli::FORMAT_R16_SFLOAT_PACK16:
      format = VK_FORMAT_R16_SFLOAT;
      break;
    case gli::FORMAT_RG16_UNORM_PACK16:
      format = VK_FORMAT_R16G16_UNORM;
      break;
    case gli::FORMAT_RG16_SNORM_PACK16:
      format = VK_FORMAT_R16G16_SNORM;
      break;
    case gli::FORMAT_RG16_USCALED_PACK16:
      format = VK_FORMAT_R16G16_USCALED;
      break;
    case gli::FORMAT_RG16_SSCALED_PACK16:
      format = VK_FORMAT_R16G16_SSCALED;
      break;
    case gli::FORMAT_RG16_UINT_PACK16:
      format = VK_FORMAT_R16G16_UINT;
      break;
    case gli::FORMAT_RG16_SINT_PACK16:
      format = VK_FORMAT_R16G16_SINT;
      break;
    case gli::FORMAT_RG16_SFLOAT_PACK16:
      format = VK_FORMAT_R16G16_SFLOAT;
      break;
    case gli::FORMAT_RGB16_UNORM_PACK16:
      format = VK_FORMAT_R16G16B16_UNORM;
      break;
    case gli::FORMAT_RGB16_SNORM_PACK16:
      format = VK_FORMAT_R16G16B16_SNORM;
      break;
    case gli::FORMAT_RGB16_USCALED_PACK16:
      format = VK_FORMAT_R16G16B16_USCALED;
      break;
    case gli::FORMAT_RGB16_SSCALED_PACK16:
      format = VK_FORMAT_R16G16B16_SSCALED;
      break;
    case gli::FORMAT_RGB16_UINT_PACK16:
      format = VK_FORMAT_R16G16B16_UINT;
      break;
    case gli::FORMAT_RGB16_SINT_PACK16:
      format = VK_FORMAT_R16G16B16_SINT;
      break;
    case gli::FORMAT_RGB16_SFLOAT_PACK16:
      format = VK_FORMAT_R16G16B16_SFLOAT;
      break;
    case gli::FORMAT_RGBA16_UNORM_PACK16:
      format = VK_FORMAT_R16G16B16A16_UNORM;
      break;
    case gli::FORMAT_RGBA16_SNORM_PACK16:
      format = VK_FORMAT_R16G16B16A16_SNORM;
      break;
    case gli::FORMAT_RGBA16_USCALED_PACK16:
      format = VK_FORMAT_R16G16B16A16_USCALED;
      break;
    case gli::FORMAT_RGBA16_SSCALED_PACK16:
      format = VK_FORMAT_R16G16B16A16_SSCALED;
      break;
    case gli::FORMAT_RGBA16_UINT_PACK16:
      format = VK_FORMAT_R16G16B16A16_UINT;
      break;
    case gli::FORMAT_RGBA16_SINT_PACK16:
      format = VK_FORMAT_R16G16B16A16_SINT;
      break;
    case gli::FORMAT_RGBA16_SFLOAT_PACK16:
      format = VK_FORMAT_R16G16B16A16_SFLOAT;
      break;
    case gli::FORMAT_R32_UINT_PACK32:
      format = VK_FORMAT_R32_UINT;
      break;
    case gli::FORMAT_R32_SINT_PACK32:
      format = VK_FORMAT_R32_SINT;
      break;
    case gli::FORMAT_R32_SFLOAT_PACK32:
      format = VK_FORMAT_R32_SFLOAT;
      break;
    case gli::FORMAT_RG32_UINT_PACK32:
      format = VK_FORMAT_R32G32_UINT;
      break;
    case gli::FORMAT_RG32_SINT_PACK32:
      format = VK_FORMAT_R32G32_SINT;
      break;
    case gli::FORMAT_RG32_SFLOAT_PACK32:
      format = VK_FORMAT_R32G32_SFLOAT;
      break;
    case gli::FORMAT_RGB32_UINT_PACK32:
      format = VK_FORMAT_R32G32B32_UINT;
      break;
    case gli::FORMAT_RGB32_SINT_PACK32:
      format = VK_FORMAT_R32G32B32_SINT;
      break;
    case gli::FORMAT_RGB32_SFLOAT_PACK32:
      format = VK_FORMAT_R32G32B32_SFLOAT;
      break;
    case gli::FORMAT_RGBA32_UINT_PACK32:
      format = VK_FORMAT_R32G32B32A32_UINT;
      break;
    case gli::FORMAT_RGBA32_SINT_PACK32:
      format = VK_FORMAT_R32G32B32A32_SINT;
      break;
    case gli::FORMAT_RGBA32_SFLOAT_PACK32:
      format = VK_FORMAT_R32G32B32A32_SFLOAT;
      break;
    case gli::FORMAT_R64_UINT_PACK64:
      format = VK_FORMAT_R64_UINT;
      break;
    case gli::FORMAT_R64_SINT_PACK64:
      format = VK_FORMAT_R64_SINT;
      break;
    case gli::FORMAT_R64_SFLOAT_PACK64:
      format = VK_FORMAT_R64_SFLOAT;
      break;
    case gli::FORMAT_RG64_UINT_PACK64:
      format = VK_FORMAT_R64G64_UINT;
      break;
    case gli::FORMAT_RG64_SINT_PACK64:
      format = VK_FORMAT_R64G64_SINT;
      break;
    case gli::FORMAT_RG64_SFLOAT_PACK64:
      format = VK_FORMAT_R64G64_SFLOAT;
      break;
    case gli::FORMAT_RGB64_UINT_PACK64:
      format = VK_FORMAT_R64G64B64_UINT;
      break;
    case gli::FORMAT_RGB64_SINT_PACK64:
      format = VK_FORMAT_R64G64B64_SINT;
      break;
    case gli::FORMAT_RGB64_SFLOAT_PACK64:
      format = VK_FORMAT_R64G64B64_SFLOAT;
      break;
    case gli::FORMAT_RGBA64_UINT_PACK64:
      format = VK_FORMAT_R64G64B64A64_UINT;
      break;
    case gli::FORMAT_RGBA64_SINT_PACK64:
      format = VK_FORMAT_R64G64B64A64_SINT;
      break;
    case gli::FORMAT_RGBA64_SFLOAT_PACK64:
      format = VK_FORMAT_R64G64B64A64_SFLOAT;
      break;
    case gli::FORMAT_RG11B10_UFLOAT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB9E5_UFLOAT_PACK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_D16_UNORM_PACK16:
      format = VK_FORMAT_D16_UNORM;
      break;
    case gli::FORMAT_D24_UNORM_PACK32:
      format = VK_FORMAT_X8_D24_UNORM_PACK32;
      break;
    case gli::FORMAT_D32_SFLOAT_PACK32:
      format = VK_FORMAT_D32_SFLOAT;
      break;
    case gli::FORMAT_S8_UINT_PACK8:
      format = VK_FORMAT_S8_UINT;
      break;
    case gli::FORMAT_D16_UNORM_S8_UINT_PACK32:
      format = VK_FORMAT_D16_UNORM_S8_UINT;
      break;
    case gli::FORMAT_D24_UNORM_S8_UINT_PACK32:
      format = VK_FORMAT_D24_UNORM_S8_UINT;
      break;
    case gli::FORMAT_D32_SFLOAT_S8_UINT_PACK64:
      format = VK_FORMAT_D32_SFLOAT_S8_UINT;
      break;
    case gli::FORMAT_RGB_DXT1_UNORM_BLOCK8:
      format = VK_FORMAT_BC1_RGB_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGB_DXT1_SRGB_BLOCK8:
      format = VK_FORMAT_BC1_RGB_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT1_UNORM_BLOCK8:
      format = VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT1_SRGB_BLOCK8:
      format = VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT3_UNORM_BLOCK16:
      format = VK_FORMAT_BC2_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT3_SRGB_BLOCK16:
      format = VK_FORMAT_BC2_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT5_UNORM_BLOCK16:
      format = VK_FORMAT_BC3_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_DXT5_SRGB_BLOCK16:
      format = VK_FORMAT_BC3_SRGB_BLOCK;
      break;
    case gli::FORMAT_R_ATI1N_UNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_R_ATI1N_SNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RG_ATI2N_UNORM_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RG_ATI2N_SNORM_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_BP_UFLOAT_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_BP_SFLOAT_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_BP_UNORM_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_BP_SRGB_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_ETC2_UNORM_BLOCK8:
      format = VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGB_ETC2_SRGB_BLOCK8:
      format = VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK8:
      format = VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK8:
      format = VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ETC2_UNORM_BLOCK16:
      format = VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ETC2_SRGB_BLOCK16:
      format = VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
      break;
    case gli::FORMAT_R_EAC_UNORM_BLOCK8:
      format = VK_FORMAT_EAC_R11_UNORM_BLOCK;
      break;
    case gli::FORMAT_R_EAC_SNORM_BLOCK8:
      format = VK_FORMAT_EAC_R11_SNORM_BLOCK;
      break;
    case gli::FORMAT_RG_EAC_UNORM_BLOCK16:
      format = VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
      break;
    case gli::FORMAT_RG_EAC_SNORM_BLOCK16:
      format = VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_4X4_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_4X4_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_5X4_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_5X4_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_5X5_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_5X5_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_6X5_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_6X5_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_6X6_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_6X6_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X5_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X5_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X6_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X6_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X8_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_8X8_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X5_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X5_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X6_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X6_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X8_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X8_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X10_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_10X10_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_12X10_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_12X10_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_12X12_UNORM_BLOCK16:
      format = VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
      break;
    case gli::FORMAT_RGBA_ASTC_12X12_SRGB_BLOCK16:
      format = VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
      break;
    case gli::FORMAT_RGB_PVRTC1_8X8_UNORM_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_PVRTC1_8X8_SRGB_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_PVRTC1_16X8_UNORM_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_PVRTC1_16X8_SRGB_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC1_8X8_UNORM_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC1_8X8_SRGB_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC1_16X8_UNORM_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;

    case gli::FORMAT_RGBA_PVRTC1_16X8_SRGB_BLOCK32:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC2_4X4_UNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC2_4X4_SRGB_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC2_8X4_UNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_PVRTC2_8X4_SRGB_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_ETC_UNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGB_ATC_UNORM_BLOCK8:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_ATCA_UNORM_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_RGBA_ATCI_UNORM_BLOCK16:
      format = VK_FORMAT_UNDEFINED;
      break;
    case gli::FORMAT_L8_UNORM_PACK8:
      format = VK_FORMAT_R8_UNORM;
      break;
    case gli::FORMAT_A8_UNORM_PACK8:
      format = VK_FORMAT_R8_UNORM;
      break;
    case gli::FORMAT_LA8_UNORM_PACK8:
      format = VK_FORMAT_R8G8_UNORM;
      break;
    case gli::FORMAT_L16_UNORM_PACK16:
      format = VK_FORMAT_R16_UNORM;
      break;
    case gli::FORMAT_A16_UNORM_PACK16:
      format = VK_FORMAT_R16_UNORM;
      break;
    case gli::FORMAT_LA16_UNORM_PACK16:
      format = VK_FORMAT_R16G16_UNORM;
      break;
    case gli::FORMAT_BGR8_UNORM_PACK32:
      format = VK_FORMAT_B8G8R8_UNORM;
      break;
    case gli::FORMAT_BGR8_SRGB_PACK32:
      format = VK_FORMAT_B8G8R8_SRGB;
      break;
    case gli::FORMAT_RG3B2_UNORM_PACK8:
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