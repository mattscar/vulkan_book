#include "framebuffer_utils.h"

void create_framebuffers(Context *ctx) {
  uint32_t attachment_count = (ctx->depth_test) ? 2 : 1;

  if(ctx->depth_test == VK_TRUE) {

    /* Create image containing depth/stencil data */
    create_image(ctx, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT 
      | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, ctx->depth_format, 
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 0, NULL,
      &ctx->depth_stencil_memory, &ctx->depth_stencil_image);

    /* Create image view */
    VkImageViewCreateInfo depth_stencil_info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .image = ctx->depth_stencil_image,
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = ctx->depth_format,
      .components = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY},
      .subresourceRange = {
        .aspectMask = 
         VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1}
    };
    ck(vkCreateImageView(ctx->device, &depth_stencil_info, 
      NULL, &ctx->depth_stencil_view));
  }

  /* Allocate memory for image views and framebuffers */
  ctx->image_views = 
    (VkImageView*)malloc(ctx->num_images * sizeof(VkImageView));
  ctx->framebuffers = (VkFramebuffer*)
    malloc(ctx->num_images * sizeof(VkFramebuffer));

  /* Include other image views if necessary */
  VkImageView *view_container =
    (VkImageView*)malloc(attachment_count * sizeof(VkImageView));
  if(ctx->depth_test) {
    view_container[1] = ctx->depth_stencil_view;
  }

  /* Create image views and framebuffers */
  for(uint32_t i=0; i<ctx->num_images; i++) {

    /* Create an image view */
    VkImageViewCreateInfo image_view_info = {
      .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .image = ctx->images[i],
      .viewType = VK_IMAGE_VIEW_TYPE_2D,
      .format = ctx->image_format.format,
      .components = {
        .r = VK_COMPONENT_SWIZZLE_IDENTITY,
        .g = VK_COMPONENT_SWIZZLE_IDENTITY,
        .b = VK_COMPONENT_SWIZZLE_IDENTITY,
        .a = VK_COMPONENT_SWIZZLE_IDENTITY
      },
      .subresourceRange = {
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1
      }
    };
    ck(vkCreateImageView(ctx->device, &image_view_info,
      NULL, &ctx->image_views[i]));
    view_container[0] = ctx->image_views[i];

    /* Create a framebuffer */
    VkFramebufferCreateInfo framebuffer_create_info = {
      .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .renderPass = ctx->render_pass,
      .attachmentCount = attachment_count,
      .pAttachments = view_container,
      .width = ctx->width,
      .height = ctx->height,
      .layers = 1
    };
    ck(vkCreateFramebuffer(ctx->device, &framebuffer_create_info,
      NULL, &ctx->framebuffers[i]));
  }

  free(view_container);
}

/* Deallocate the render pass */
void destroy_framebuffers(Context* ctx) {

  /* Destroy image views and framebuffers */
  for(uint32_t i=0; i<ctx->num_images; i++) {
    vkDestroyFramebuffer(ctx->device, ctx->framebuffers[i], NULL);
    vkDestroyImageView(ctx->device, ctx->image_views[i], NULL);
  }

  /* Deallocate depth-related resources */
  if(ctx->depth_test) {
    vkDestroyImageView(ctx->device, ctx->depth_stencil_view, NULL);
    destroy_image(ctx, &ctx->depth_stencil_image, &ctx->depth_stencil_memory);
  }

  free(ctx->framebuffers);
  free(ctx->image_views);
}