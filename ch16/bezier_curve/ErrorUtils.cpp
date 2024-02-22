#include <iostream>

#include "ErrorUtils.hpp"

// Display debug message 
vk::Bool32 messageCallbackFunc(vk::DebugReportFlagsEXT flags,
    vk::DebugReportObjectTypeEXT type, uint64_t object,
    size_t location, int code, const char* layer,
	  const char* message, void* user_data) {

  // Print message type 
  if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    std::cout << "Info ";
  }
  if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    std::cout << "Warning ";
  }
  if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    std::cout << "Performance_Warning ";
  }
  if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    std::cout << "Debug ";
  }
  if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    std::cout << "Error ";
  }

  // Print message 
  std::cout << ": " << message << std::endl;
	return VK_FALSE;
}

void setupValidation(std::vector<const char*>& layerNames,
                     std::vector<const char*>& extNames) {

  std::vector<vk::LayerProperties> layerProps;
  std::vector<vk::ExtensionProperties> extProps;

  // Obtain layer properties 
  layerProps = vk::enumerateInstanceLayerProperties();

  // Add validation layers
  for(vk::LayerProperties layerProp: layerProps) {

    // Add core validation layer
    if(!strcmp(layerProp.layerName, "VK_LAYER_LUNARG_core_validation")) {
      
      std::cout << "Layer name 0: " << layerProp.layerName << std::endl;
      
      layerNames.push_back("VK_LAYER_LUNARG_core_validation");
    }
    
    // Add core validation layer
    if(!strcmp(layerProp.layerName, "VK_LAYER_LUNARG_parameter_validation")) {
      
      std::cout << "Layer name 1: " << layerProp.layerName << std::endl;      
      
      layerNames.push_back("VK_LAYER_LUNARG_parameter_validation");
    }
  }
  
  // Obtain extension properties
  extProps = vk::enumerateInstanceExtensionProperties();

  // Check if the debug extension is supported 
  for(vk::ExtensionProperties extProp: extProps) {

    // Add debug extension
    if(strcmp(extProp.extensionName, "VK_EXT_debug_report")) {
      extNames.push_back("VK_EXT_debug_report");
      break;
    }
  }
}

void setupReporting(Context& ctx, std::vector<const char*>& extNames) {

  // Make sure debug reporting is available 
  if(std::find(extNames.begin(), extNames.end(), 
    "VK_EXT_debug_report") != extNames.end()) {
    
    // Assign addresses
    PFN_vkCreateDebugReportCallbackEXT createCallback =
      (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(ctx.instance, 
      "vkCreateDebugReportCallbackEXT");
    ctx.destroyFunc = (PFN_vkDestroyDebugReportCallbackEXT)
      vkGetInstanceProcAddr(ctx.instance, "vkDestroyDebugReportCallbackEXT");
      
    // Create the reporting configuration info 
    vk::DebugReportCallbackCreateInfoEXT debugInfo = {
      .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
      .pNext = NULL,
      .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | 
        VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
        VK_DEBUG_REPORT_ERROR_BIT_EXT | 
        VK_DEBUG_REPORT_DEBUG_BIT_EXT,
      .pfnCallback = &messageCallbackFunc,
      .pUserData = NULL
    };

    // Configure reporting 
    createCallback(ctx.instance, &debugInfo, NULL, &ctx.callback);    
  }
}

#ifndef DEBUG

template<typename T>
T ck(vk::ResultValueType<T> result) { return result.value; }

void destroyReporting(Context& ctx) {}

#else

template<typename T>
T ck(vk::ResultValueType<T> result) {

  // Return if successful 
  if(result.result == vk::Result::eSuccess) {
    return result.value;
  }

  // Print error message 
  switch(result.result) {
    case vk::Result::eNotReady:
      std::cerr << "Error: VK_NOT_READY" << std::endl;
      break;

    case vk::Result::eTimeout:
      std::cerr << "Error: VK_TIMEOUT" << std::endl;
      break;

    case vk::Result::eEventSet:
      std::cerr << "Error: VK_EVENT_SET" << std::endl;
      break;

    case vk::Result::eEventReset:
      std::cerr << "Error: VK_EVENT_RESET" << std::endl;
      break;

    case vk::Result::eIncomplete:
      std::cerr << "Error: VK_INCOMPLETE" << std::endl;
      break;

    case vk::Result::eErrorOutOfHostMemory:
      std::cerr << "Error: VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
      break;

    case vk::Result::eErrorOutOfDeviceMemory:
      std::cerr << "Error: VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
      break;

    case vk::Result::eErrorInitializationFailed:
      std::cerr << "Error: VK_ERROR_INITIALIZATION_FAILED" << std::endl;
      break;

    case vk::Result::eErrorDeviceLost:
      std::cerr << "Error: VK_ERROR_DEVICE_LOST" << std::endl;
      break;

    case vk::Result::eErrorMemoryMapFailed:
      std::cerr << "Error: VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
      break;

    case vk::Result::eErrorLayerNotPresent:
      std::cerr << "Error: VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
      break;

    case vk::Result::eErrorExtensionNotPresent:
      std::cerr << "Error: VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
      break;

    case vk::Result::eErrorFeatureNotPresent:
      std::cerr << "Error: VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
      break;

    case vk::Result::eErrorIncompatibleDriver:
      std::cerr << "Error: VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
      break;

    case vk::Result::eErrorTooManyObjects:
      std::cerr << "Error: VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
      break;

    case vk::Result::eErrorFormatNotSupported:
      std::cerr << "Error: VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
      break;

    case vk::Result::eErrorSurfaceLostKHR:
      std::cerr << "Error: VK_ERROR_SURFACE_LOST_KHR" << std::endl;
      break;

    case vk::Result::eErrorNativeWindowInUseKHR:
      std::cerr << "Error: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
      break;

    case vk::Result::eSuboptimalKHR:
      std::cerr << "Error: VK_SUBOPTIMAL_KHR" << std::endl;
      break;

    case vk::Result::eErrorOutOfDateKHR:
      std::cerr << "Error: VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
      break;

    case vk::Result::eErrorIncompatibleDisplayKHR:
      std::cerr << "Error: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
      break;

    case vk::Result::eErrorValidationFailedEXT:
      std::cerr << "Error: VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
      break;

    default:
      std::cerr << "Error: Unknown error" << std::endl;
      break;
  }

  exit(EXIT_FAILURE);
}

void destroyReporting(Context& ctx) {

  if(ctx.destroyFunc) {
    ctx.destroyFunc(ctx.instance, ctx.callback, NULL);
  }
}
#endif