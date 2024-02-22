#include "render_pass_utils.h"

void create_render_pass(Context* ctx, uint32_t num_clear_values,
  VkClearValue* clear_values) {

  /* Color attachment description */
  VkAttachmentDescription color_description = {
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

  /* Subpass descriptions */
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

  /* Create render pass */
  VkRenderPassCreateInfo render_pass_create_info = {
    .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
    .pNext = NULL,
    .flags = 0,
    .attachmentCount = 1,
    .pAttachments = &color_description,
    .subpassCount = 1,
    .pSubpasses = &subpass_description,
    .dependencyCount = 0,
    .pDependencies = NULL
  };

  /* Create the render_pass */
  ck(vkCreateRenderPass(ctx->device, &render_pass_create_info,
    NULL, &ctx->render_pass));

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