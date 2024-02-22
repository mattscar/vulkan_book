#include "device_utils.h"

void create_device(Context* ctx, uint32_t num_queues, 
  VkQueueFlagBits queue_flags[], const char* app_name) {

  VkPhysicalDevice* devices;
  VkPhysicalDeviceProperties device_props;
  VkQueueFamilyProperties* family_props;
  VkDeviceQueueCreateInfo* queue_infos;
  uint32_t i, j, dev_count, family_count;
  int32_t dev_index, family_index;

  /* Instance layers and extensions */
  uint32_t num_instance_layers = 0, num_instance_ext = 2;
  char* instance_layer_names[] = {};
  char* instance_ext_names[] = {(char*)"VK_KHR_surface", (char*)"VK_KHR_xcb_surface"};

  /* Device layers and extensions */
  uint32_t num_device_layers = 0, num_device_ext = 1;
  char* device_layer_names[] = {};
  char* device_ext_names[] = {(char*)"VK_KHR_swapchain"};

#ifdef DEBUG
  /* Configure validation and error checking */
  setup_validation(&num_instance_layers, instance_layer_names, 
    &num_instance_ext, instance_ext_names);  
#endif    
    
  /* Application data */
  VkApplicationInfo app_info = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pNext = NULL,
    .pApplicationName = app_name,
    .applicationVersion = 1,
    .pEngineName = NULL,
    .engineVersion = 1,
    .apiVersion = VK_MAKE_VERSION(1, 0, 3)
  };

  /* Instance data */
  VkInstanceCreateInfo instance_info = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .pApplicationInfo = &app_info,
    .enabledLayerCount = num_instance_layers,
    .ppEnabledLayerNames = (const char* const*)instance_layer_names,
    .enabledExtensionCount = num_instance_ext,
    .ppEnabledExtensionNames = (const char* const*)instance_ext_names
  };
  
  /* Create an instance */
  ck(vkCreateInstance(&instance_info, NULL, &ctx->instance));

#ifdef DEBUG
  /* Configure validation and error checking */
  setup_reporting(ctx, num_instance_ext, instance_ext_names);
#endif      
  
  /* Get physical devices */
  ck(vkEnumeratePhysicalDevices(ctx->instance, &dev_count, NULL));

  /* Allocate memory and initialize devices */
  devices = (VkPhysicalDevice*)malloc(dev_count* sizeof(VkPhysicalDevice));
  ck(vkEnumeratePhysicalDevices(ctx->instance, &dev_count, devices));

  /* Iterate through devices to find a GPU */
  dev_index = -1;
  for(i=0; i<dev_count; i++) {
    vkGetPhysicalDeviceProperties(devices[i], &device_props);
    if(device_props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
         || VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU
         || VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU) {
      dev_index = i;
      break;
    }
  }

  /* Select the physical device corresponding to the GPU */
  if(dev_index == -1) {
    fprintf(stderr, "Error: couldn't find GPU.");
    free(devices);
    exit(1);
  }
  ctx->phys_device = devices[dev_index];
  free(devices);

  /* Determine number of queue families */
  vkGetPhysicalDeviceQueueFamilyProperties(ctx->phys_device, &family_count, NULL);

  /* Allocate memory and initialize family properties */
  family_props = (VkQueueFamilyProperties*)malloc(family_count * sizeof(VkQueueFamilyProperties));
  vkGetPhysicalDeviceQueueFamilyProperties(ctx->phys_device, &family_count, family_props);

  /* Allocate memory for queue information structures */
  queue_infos = (VkDeviceQueueCreateInfo*)malloc(num_queues * sizeof(VkDeviceQueueCreateInfo));
  
  /* Iterate through queues */
  for(i=0; i<num_queues; i++) {
  
    /* Find queue family that supports the given flag */
    family_index = -1;
    for(j=0; j<family_count; j++) {
      if(family_props[j].queueFlags & queue_flags[i]) {
        family_index = j;
        break;
      }
    }

    /* Exit if no suitable queue family can be found */
    if (family_index == -1) {
      fprintf(stderr, "Error: couldn't find suitable queue family.");
      free(family_props);
      exit(1);
    }
    ctx->family_indices[i] = family_index;
    
    /* Queue info */
    queue_infos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_infos[i].pNext = NULL;
    queue_infos[i].flags = 0;
    queue_infos[i].queueFamilyIndex = ctx->family_indices[i];
    queue_infos[i].queueCount = 1;
    queue_infos[i].pQueuePriorities = NULL;
  }
  free(family_props);

  /* Device info */
  VkDeviceCreateInfo device_info = {
    .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .queueCreateInfoCount = num_queues,
    .pQueueCreateInfos = queue_infos,
    .enabledLayerCount = num_device_layers,
    .ppEnabledLayerNames = (const char* const*)device_layer_names,
    .enabledExtensionCount = num_device_ext,
    .ppEnabledExtensionNames = (const char* const*)device_ext_names,
    .pEnabledFeatures = NULL
  };

  /* Create logical device */
  ck(vkCreateDevice(ctx->phys_device, &device_info, NULL, &ctx->device));
  free(queue_infos);
  
  /* Access one queue for each family */
  for(i=0; i<num_queues; i++) {
    vkGetDeviceQueue(ctx->device, ctx->family_indices[i], 0, &ctx->queues[i]);
  }
}

/* Deallocate structures */
void destroy_device(Context* ctx) {
  
#ifdef DEBUG
  destroy_reporting(ctx);
#endif     
  
  vkDestroyDevice(ctx->device, NULL);
  vkDestroyInstance(ctx->instance, NULL);
}