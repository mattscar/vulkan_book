#include "framebuffer_utils.h"

void create_framebuffers(Context *ctx) {

  /* Allocate memory for image views and framebuffers */
  ctx->image_views = (VkImageView*)malloc(ctx->num_images * sizeof(VkImageView));
  ctx->framebuffers = (VkFramebuffer*)malloc(ctx->num_images * sizeof(VkFramebuffer));
  
  /* Create image views and framebuffers */
  for(uint32_t i=0; i<ctx->num_images; i++) {

    /* Create an image view */
    create_image_view(ctx, ctx->image_format.format, VK_IMAGE_ASPECT_COLOR_BIT, 
      &ctx->images[i], &ctx->image_views[i]);

    /* Create a framebuffer */
    VkFramebufferCreateInfo framebuffer_create_info = {
      .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .pNext = NULL,
      .flags = 0,
      .renderPass = ctx->render_pass,
      .attachmentCount = 1,
      .pAttachments = &ctx->image_views[i],
      .width = ctx->width,
      .height = ctx->height,
      .layers = 1
    };
    ck(vkCreateFramebuffer(ctx->device, &framebuffer_create_info,
      NULL, &ctx->framebuffers[i]));
  }
}

/* Deallocate the render pass */
void destroy_framebuffers(Context* ctx) {

  /* Destroy image views and framebuffers */
  for(uint32_t i=0; i<ctx->num_images; i++) {
    vkDestroyFramebuffer(ctx->device, ctx->framebuffers[i], NULL);
    vkDestroyImageView(ctx->device, ctx->image_views[i], NULL);
  }

  free(ctx->framebuffers);
  free(ctx->image_views);
}