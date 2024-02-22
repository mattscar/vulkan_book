#include "surface_utils.h"

void create_xcb_surface(Context* ctx);
void get_surface_formats(Context* ctx);

void create_surface(Context* ctx) {
  
#ifdef VK_USE_PLATFORM_XCB_KHR
  create_xcb_surface(ctx);
#endif

  get_surface_formats(ctx);
}

#ifdef VK_USE_PLATFORM_XCB_KHR
void create_xcb_surface(Context* ctx) {

  const xcb_setup_t *setup;
  xcb_screen_t *screen;
  uint32_t value_mask;
  uint32_t value_list[2];

  /* Connect to X server */
  ctx->display = xcb_connect(NULL, NULL);
  if(xcb_connection_has_error(ctx->display)) {
    printf("Error opening display.\n");
    exit(1);
  }

  /* Obtain setup info and access the screen */
  setup = xcb_get_setup(ctx->display);
  screen = xcb_setup_roots_iterator(setup).data;

  /* Define window's configuration */
  ctx->window = xcb_generate_id(ctx->display);
  value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  value_list[0] = screen->white_pixel;
  value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
    XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_STRUCTURE_NOTIFY;

  /* Create window */
  xcb_create_window(
    ctx->display,             /* The connection */
    XCB_COPY_FROM_PARENT,     /* The screen depth */
    ctx->window,              /* The window's ID */
    screen->root,             /* The root visual */
    ctx->x, ctx->y,           /* Top-left coordinate */
    ctx->width, ctx->height,  /* Window dimensions */
    0,                        /* Border width */
    XCB_WINDOW_CLASS_INPUT_OUTPUT,   /* Window class */
    screen->root_visual,      /* The root visual */
    value_mask,               /* Names of window properties */
    value_list);              /* Values of window properties */

  /* Set window title */
  xcb_change_property(ctx->display, XCB_PROP_MODE_REPLACE, ctx->window, 
    XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(ctx->title), ctx->title); 
  xcb_flush(ctx->display);   
    
  /* Create the surface */
  VkXcbSurfaceCreateInfoKHR surface_info = {
    .sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
    .pNext = NULL,
    .flags = 0,
    .connection = ctx->display,
    .window = ctx->window
  };
  vkCreateXcbSurfaceKHR(ctx->instance, &surface_info, NULL, &ctx->surface);
}

void get_surface_formats(Context* ctx) {
  
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes;  
  uint32_t i, num_formats, num_modes;  
  
  /* Obtain image formats */
  ck(vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->phys_device, ctx->surface, &num_formats, NULL));
  formats = (VkSurfaceFormatKHR*)malloc(num_formats * sizeof(VkSurfaceFormatKHR));
  ck(vkGetPhysicalDeviceSurfaceFormatsKHR(ctx->phys_device, ctx->surface, &num_formats, formats));

  /* Select 8-bit RGBA format if available */
  for(i = 0; i < num_formats; i++) {
    if (formats[i].format == VK_FORMAT_R8G8B8A8_UNORM) {
      ctx->image_format = formats[i];
      break;
    }
  }
  if (ctx->image_format.format != VK_FORMAT_R8G8B8A8_UNORM) {
    ctx->image_format = formats[0];
  }
  
  /* Get presentation modes */
  ck(vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->phys_device, ctx->surface, &num_modes, NULL));
  modes = (VkPresentModeKHR*)malloc(num_modes * sizeof(VkPresentModeKHR));
  ck(vkGetPhysicalDeviceSurfacePresentModesKHR(ctx->phys_device, ctx->surface, &num_modes, modes));
  
  /* Select mailbox mode if available */
  for(i = 0; i < num_modes; i++) {
    if (modes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
      ctx->presentation_mode = modes[i];
      break;
    }
  }
  if (ctx->presentation_mode != VK_PRESENT_MODE_MAILBOX_KHR) {
    ctx->presentation_mode = VK_PRESENT_MODE_FIFO_KHR;
  }
  
  /* Deallocate memory */
  free(formats);
  free(modes);  
}

/* Display the window */
void display_surface(Context* ctx) {

  /* Obtain atom for WM_PROTOCOLS */
  xcb_intern_atom_cookie_t proto_cookie = 
    xcb_intern_atom(ctx->display, 1, 12, "WM_PROTOCOLS" );
  xcb_intern_atom_reply_t *proto_reply = 
    xcb_intern_atom_reply(ctx->display, proto_cookie, 0 );
  xcb_atom_t proto_atom = (*proto_reply).atom;

  /* Obtain atom for WM_DELETE_WINDOW */
  xcb_intern_atom_cookie_t close_cookie = 
    xcb_intern_atom(ctx->display, 0, 16, "WM_DELETE_WINDOW" );
  xcb_intern_atom_reply_t *close_reply = 
    xcb_intern_atom_reply(ctx->display, close_cookie, 0 );
  xcb_atom_t close_atom = (*close_reply).atom;

  /* Update the WM_PROTOCOLS property to send close messages */
  xcb_change_property(ctx->display, XCB_PROP_MODE_REPLACE, 
    ctx->window, proto_atom, 4, 32, 1, &close_atom);
  free(proto_reply);

  /* Display the window */
  xcb_map_window(ctx->display, ctx->window);
  xcb_flush(ctx->display);
  xcb_generic_event_t *event;
  VkBool32 loop = VK_TRUE;

  while(loop) {
    event = xcb_poll_for_event(ctx->display);
    if(event) {

      /* Check event type */
      switch (event->response_type & 0x7f) {

        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t *conf_event = (xcb_configure_notify_event_t*)event;

            if(((conf_event->width > 0) && (conf_event->width != ctx->width)) ||
              ((conf_event->height > 0) && (conf_event->height != ctx->height))) {

              free(event);              
              resize_window(ctx);
              return;
            }
          }
          break;

        /* Close window on key press */
        case XCB_KEY_PRESS:
          loop = VK_FALSE;
          break;

        /* Process message */
        case XCB_CLIENT_MESSAGE:

          /* Check for close window atom */
          if((*(xcb_client_message_event_t*)event).data.data32[0] == close_atom) {
            free(close_reply);            
            loop = VK_FALSE;
          }
          break;
      }
      free(event);

    } else {
      draw(ctx);
    }
  }

  /* Deallocate structures */
  destroy(ctx);
}

/* Deallocate surface structures */
void destroy_surface(Context* ctx) {

  xcb_disconnect(ctx->display);  
  vkDestroySurfaceKHR(ctx->instance, ctx->surface, NULL);
}
#endif