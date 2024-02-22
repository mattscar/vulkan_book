#include "error_utils.h"

/* Display debug message */
VkBool32 message_callback_func(VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT type, uint64_t object,
    size_t location, int code, const char* layer,
	  const char* message, void* user_data) {

  /* Print message type */
  if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
    printf("Info ");
  }
  if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
    printf("Warning ");
  }
  if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
    printf("Performance_Warning ");
  }
  if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
    printf("Debug ");
  }
  if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
    printf("Error ");
  }

  /* Print message */
  printf(": %s\n", message);
	return VK_FALSE;
}

void setup_validation(uint32_t* num_layers, char **layer_names,
                      uint32_t* num_ext, char **ext_names) {

  uint32_t layer_count, ext_count, i;
  VkLayerProperties* layer_props;
  VkExtensionProperties* ext_props;
  int validation_supported = 0;

  /* Allocate and obtain layer properties */
  vkEnumerateInstanceLayerProperties(&layer_count, NULL);
  layer_props = (VkLayerProperties*)malloc(layer_count* sizeof(VkLayerProperties));
  vkEnumerateInstanceLayerProperties(&layer_count, layer_props);

  /* Check if the validation layer is supported */
  for(i = 0; i < layer_count; i++) {
    if(strcmp(layer_props[i].layerName, "VK_LAYER_LUNARG_standard_validation") == 0) {
      validation_supported = 1;
      break;
    }
  }

  if(validation_supported) {

    /* Allocate and obtain extension properties */
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, NULL);
    ext_props = (VkExtensionProperties*)malloc(ext_count* sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &ext_count, ext_props);

    /* Check if the debug extension is supported */
    for(i = 0; i < ext_count; i++) {
      if(strcmp(ext_props[i].extensionName, "VK_EXT_debug_report") == 0) {

        layer_names[*num_layers] = (char*)malloc(64 * sizeof(char));
        layer_names[*num_layers] = (char*)"VK_LAYER_LUNARG_core_validation";
        (*num_layers)++;

        layer_names[*num_layers] = (char*)malloc(64 * sizeof(char));
        layer_names[*num_layers] = (char*)"VK_LAYER_LUNARG_parameter_validation";
        (*num_layers)++;

        ext_names[*num_ext] = (char*)malloc(32 * sizeof(char));
        ext_names[*num_ext] = (char*)"VK_EXT_debug_report";
        (*num_ext)++;
        break;
      }
    }
  }

  /* Deallocate arrays */
  free(layer_props);
  free(ext_props);
}

void setup_reporting(Context* ctx, uint32_t num_ext, char **ext_names) {

  uint32_t i, reporting_supported = 0;

  /* Make sure debug reporting is available */
  for(i = 0; i < num_ext; i++) {
    if(strcmp(ext_names[i], "VK_EXT_debug_report") == 0) {
      reporting_supported = 1;
      break;
    }
  }
  if(!reporting_supported) {
    fprintf(stderr, "Debug reporting not supported.\n");
    return;
  }

  /* Assign addresses */
  PFN_vkCreateDebugReportCallbackEXT create_callback =
    (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(ctx->instance, "vkCreateDebugReportCallbackEXT");
  ctx->destroy_func = (PFN_vkDestroyDebugReportCallbackEXT)
    vkGetInstanceProcAddr(ctx->instance, "vkDestroyDebugReportCallbackEXT");

  /* Create the reporting configuration info */
  VkDebugReportCallbackCreateInfoEXT debug_create_info = {
    .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT,
    .pNext = NULL,
    .flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
             VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT,
    .pfnCallback = &message_callback_func,
    .pUserData = NULL
  };

  /* Configure reporting */
  create_callback(ctx->instance, &debug_create_info, NULL, &ctx->callback);
}

#ifndef DEBUG

void ck(VkResult result) {}
void destroy_reporting(Context* ctx) {}

#else

void ck(VkResult result) {

  /* Return if successful */
  if(result == VK_SUCCESS) {
    return;
  }

  /* Print error message */
  switch(result) {
    case VK_NOT_READY:
      fprintf(stderr, "Error: VK_NOT_READY\n");
      break;

    case VK_TIMEOUT:
      fprintf(stderr, "Error: VK_TIMEOUT\n");
      break;

    case VK_EVENT_SET:
      fprintf(stderr, "Error: VK_EVENT_SET\n");
      break;

    case VK_EVENT_RESET:
      fprintf(stderr, "Error: VK_EVENT_RESET\n");
      break;

    case VK_INCOMPLETE:
      fprintf(stderr, "Error: VK_INCOMPLETE\n");
      break;

    case VK_ERROR_OUT_OF_HOST_MEMORY:
      fprintf(stderr, "Error: VK_ERROR_OUT_OF_HOST_MEMORY\n");
      break;

    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
      fprintf(stderr, "Error: VK_ERROR_OUT_OF_DEVICE_MEMORY\n");
      break;

    case VK_ERROR_INITIALIZATION_FAILED:
      fprintf(stderr, "Error: VK_ERROR_INITIALIZATION_FAILED\n");
      break;

    case VK_ERROR_DEVICE_LOST:
      fprintf(stderr, "Error: VK_ERROR_DEVICE_LOST\n");
      break;

    case VK_ERROR_MEMORY_MAP_FAILED:
      fprintf(stderr, "Error: VK_ERROR_MEMORY_MAP_FAILED\n");
      break;

    case VK_ERROR_LAYER_NOT_PRESENT:
      fprintf(stderr, "Error: VK_ERROR_LAYER_NOT_PRESENT\n");
      break;

    case VK_ERROR_EXTENSION_NOT_PRESENT:
      fprintf(stderr, "Error: VK_ERROR_EXTENSION_NOT_PRESENT\n");
      break;

    case VK_ERROR_FEATURE_NOT_PRESENT:
      fprintf(stderr, "Error: VK_ERROR_FEATURE_NOT_PRESENT\n");
      break;

    case VK_ERROR_INCOMPATIBLE_DRIVER:
      fprintf(stderr, "Error: VK_ERROR_INCOMPATIBLE_DRIVER\n");
      break;

    case VK_ERROR_TOO_MANY_OBJECTS:
      fprintf(stderr, "Error: VK_ERROR_TOO_MANY_OBJECTS\n");
      break;

    case VK_ERROR_FORMAT_NOT_SUPPORTED:
      fprintf(stderr, "Error: VK_ERROR_FORMAT_NOT_SUPPORTED\n");
      break;

    case VK_ERROR_SURFACE_LOST_KHR:
      fprintf(stderr, "Error: VK_ERROR_SURFACE_LOST_KHR\n");
      break;

    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
      fprintf(stderr, "Error: VK_ERROR_NATIVE_WINDOW_IN_USE_KHR\n");
      break;

    case VK_SUBOPTIMAL_KHR:
      fprintf(stderr, "Error: VK_SUBOPTIMAL_KHR\n");
      break;

    case VK_ERROR_OUT_OF_DATE_KHR:
      fprintf(stderr, "Error: VK_ERROR_OUT_OF_DATE_KHR\n");
      break;

    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
      fprintf(stderr, "Error: VK_ERROR_INCOMPATIBLE_DISPLAY_KHR\n");
      break;

    case VK_ERROR_VALIDATION_FAILED_EXT:
      fprintf(stderr, "Error: VK_ERROR_VALIDATION_FAILED_EXT\n");
      break;

    case VK_RESULT_RANGE_SIZE:
      fprintf(stderr, "Error: VK_RESULT_RANGE_SIZE\n");
      break;

    case VK_RESULT_MAX_ENUM:
      fprintf(stderr, "Error: VK_RESULT_MAX_ENUM\n");
      break;

    default:
      fprintf(stderr, "Error: Unknown error\n");
      break;
  }

  exit(EXIT_FAILURE);
}

void destroy_reporting(Context* ctx) {
  if(ctx->destroy_func != NULL) {
    ctx->destroy_func(ctx->instance, ctx->callback, NULL);
  }
}
#endif