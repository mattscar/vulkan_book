#include "render_pass_utils.h"

void create_render_pass(Context* ctx, uint32_t num_clear_values,
  VkClearValue* clear_values) {

  uint32_t attachment_count = (ctx->depth_test) ? 2 : 1;
  VkAttachmentDescription *attachment_descriptions;

  /* Allocate memory for attachment descriptions */
  attachment_descriptions = (VkAttachmentDescription*)
    malloc(attachment_count * sizeof(VkAttachmentDescription));

  /* Color attachment description */
  attachment_descriptions[0] = {
    .flags = 0,
    .format = ctx->image_format.format,
    .samples = VK_SAMPLE_COUNT_1_BIT,
    .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
    .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
    .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
    .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
  };

  /* Color attachment reference */
  VkAttachmentReference color_reference = {
    .attachment = 0,
    .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
  };

  /* Subpass description */
  VkSubpassDescription subpass_description = {
    .flags = 0,
    .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
    .inputAttachmentCount = 0,
    .pInputAttachments = NULL,
    .colorAttachmentCount = 1,
    .pColorAttachments = &color_reference,
    .pResolveAttachments = NULL,
    .pDepthStencilAttachment = NULL,
    .preserveAttachmentCount = 0,
    .pPreserveAttachments = NULL
  };

  /* Add depth testing */
  if(ctx->depth_test == VK_TRUE) {

    /* Find a format for the depth/stencil image */
    VkFormatProperties props;
		VkFormat formats[5] = {VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT, VK_FORMAT_D24_UNORM_S8_UINT,
      VK_FORMAT_D16_UNORM_S8_UINT, VK_FORMAT_D16_UNORM};
    for(uint32_t i=0; i<5; i++) {
			vkGetPhysicalDeviceFormatProperties(ctx->phys_device, formats[i], &props);
			if(props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				ctx->depth_format = formats[i];
        break;
			}
    }

    /* Depth attachment description */
    attachment_descriptions[1] = {
      .flags = 0,
      .format = ctx->depth_format,
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };

    /* Depth attachment reference */
    VkAttachmentReference depth_reference = {
      .attachment = 1,
      .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
    };
    subpass_description.pDepthStencilAttachment = &depth_reference;
  }

  /* Create render pass */
  VkRenderPassCreateInfo render_pass_create_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = attachment_count,
    .pAttachments = attachment_descriptions,
    .subpassCount = 1,
    .pSubpasses = &subpass_description,
    .dependencyCount = 0,
    .pDependencies = NULL
  };

  /* Create the render_pass */
  ck(vkCreateRenderPass(ctx->device, &render_pass_create_info,
    NULL, &ctx->render_pass));

  /* Deallocate resources */
  free(attachment_descriptions);

  /* Configure info to begin render_pass */
  ctx->render_pass_begin_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  ctx->render_pass_begin_info.pNext = NULL;
  ctx->render_pass_begin_info.renderPass = ctx->render_pass;
  ctx->render_pass_begin_info.renderArea.offset.x = ctx->x;
  ctx->render_pass_begin_info.renderArea.offset.y = ctx->y;
  ctx->render_pass_begin_info.renderArea.extent.width = ctx->width;
  ctx->render_pass_begin_info.renderArea.extent.height = ctx->height;
  ctx->render_pass_begin_info.clearValueCount = num_clear_values;
  ctx->render_pass_begin_info.pClearValues = clear_values;
}

/* Deallocate the render pass */
void destroy_render_pass(Context* ctx) {
  vkDestroyRenderPass(ctx->device, ctx->render_pass, NULL);
}