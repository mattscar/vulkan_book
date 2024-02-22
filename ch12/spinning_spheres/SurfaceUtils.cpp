#include "SurfaceUtils.hpp"

namespace surface_utils {

void createXcbSurface(Context& ctx);
void getSurfaceFormats(Context& ctx);

void createSurface(Context& ctx) {

#ifdef VK_USE_PLATFORM_XCB_KHR
  createXcbSurface(ctx);
#endif

  getSurfaceFormats(ctx);
}

#ifdef VK_USE_PLATFORM_XCB_KHR
void createXcbSurface(Context& ctx) {

  const xcb_setup_t *setup;
  xcb_screen_t *screen;
  uint32_t value_mask;
  uint32_t value_list[2];

  // Connect to X server
  ctx.display = xcb_connect(NULL, NULL);
  if(xcb_connection_has_error(ctx.display)) {
    std::cerr << "Error opening display" << std::endl;
    exit(1);
  }

  // Obtain setup info and access the screen
  setup = xcb_get_setup(ctx.display);
  screen = xcb_setup_roots_iterator(setup).data;

  // Define window's configuration
  ctx.window = xcb_generate_id(ctx.display);
  value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  value_list[0] = screen->white_pixel;
  value_list[1] = XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
    XCB_EVENT_MASK_KEY_PRESS|XCB_EVENT_MASK_STRUCTURE_NOTIFY;

  // Create window
  xcb_create_window(
    ctx.display,             // The connection
    XCB_COPY_FROM_PARENT,     // The screen depth
    ctx.window,              // The window's ID
    screen->root,             // The root visual
    ctx.x, ctx.y,           // Top-left coordinate
    ctx.width, ctx.height,  // Window dimensions
    0,                        // Border width
    XCB_WINDOW_CLASS_INPUT_OUTPUT,   // Window class
    screen->root_visual,      // The root visual
    value_mask,               // Names of window properties
    value_list);              // Values of window properties

  // Set window title
  xcb_change_property(ctx.display, XCB_PROP_MODE_REPLACE, ctx.window,
    XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, ctx.title.length(), ctx.title.c_str());
  xcb_flush(ctx.display);

  // Create the surface
  vk::XcbSurfaceCreateInfoKHR surfaceInfo = vk::XcbSurfaceCreateInfoKHR()
    .setConnection(ctx.display)
    .setWindow(ctx.window);
  auto res = ctx.instance.createXcbSurfaceKHR(surfaceInfo);
  ck(res.result);
  ctx.surface = res.value;
}

void getSurfaceFormats(Context& ctx) {

  // Obtain image formats
  auto formatRes = ctx.physDevice.getSurfaceFormatsKHR(ctx.surface);
  ck(formatRes.result);
  std::vector<vk::SurfaceFormatKHR> formats = formatRes.value;

  // Select 8-bit RGBA format if available
  for(vk::SurfaceFormatKHR fmt: formats) {
    if (fmt.format == vk::Format::eR8G8B8A8Unorm) {
      ctx.imageFormat = fmt;
      break;
    }
  }
  if (ctx.imageFormat.format != vk::Format::eR8G8B8A8Unorm) {
    ctx.imageFormat = formats[0];
  }

  // Get presentation modes
  auto modeRes = ctx.physDevice.getSurfacePresentModesKHR(ctx.surface);
  ck(modeRes.result);
  std::vector<vk::PresentModeKHR> modes = modeRes.value;

  // Select mailbox mode if available
  for(vk::PresentModeKHR mode: modes) {
    if (mode == vk::PresentModeKHR::eMailbox) {
      ctx.presentationMode = mode;
      break;
    }
  }
  if (ctx.presentationMode != vk::PresentModeKHR::eMailbox) {
    ctx.presentationMode = vk::PresentModeKHR::eFifo;
  }
}

// Display the window
void displaySurface(Context& ctx) {

  // Obtain atom for WM_PROTOCOLS
  xcb_intern_atom_cookie_t protoCookie =
    xcb_intern_atom(ctx.display, 1, 12, "WM_PROTOCOLS" );
  xcb_intern_atom_reply_t *protoReply =
    xcb_intern_atom_reply(ctx.display, protoCookie, 0 );
  xcb_atom_t proto_atom = (*protoReply).atom;

  // Obtain atom for WM_DELETE_WINDOW
  xcb_intern_atom_cookie_t closeCookie =
    xcb_intern_atom(ctx.display, 0, 16, "WM_DELETE_WINDOW" );
  xcb_intern_atom_reply_t *closeReply =
    xcb_intern_atom_reply(ctx.display, closeCookie, 0 );
  xcb_atom_t close_atom = (*closeReply).atom;

  // Update the WM_PROTOCOLS property to send close messages
  xcb_change_property(ctx.display, XCB_PROP_MODE_REPLACE,
    ctx.window, proto_atom, 4, 32, 1, &close_atom);
  free(protoReply);

  // Display the window
  xcb_map_window(ctx.display, ctx.window);
  xcb_flush(ctx.display);
  xcb_generic_event_t *event;
  vk::Bool32 loop = VK_TRUE;

  while(loop) {
    event = xcb_poll_for_event(ctx.display);
    if(event) {

      // Check event type
      switch (event->response_type & 0x7f) {

        case XCB_CONFIGURE_NOTIFY: {
            xcb_configure_notify_event_t *confEvent = (xcb_configure_notify_event_t*)event;

            if(((confEvent->width > 0) && (confEvent->width != ctx.width)) ||
              ((confEvent->height > 0) && (confEvent->height != ctx.height))) {

              free(event);
              resizeWindow(ctx);
              return;
            }
          }
          break;

        // Close window on key press
        case XCB_KEY_PRESS:
          loop = VK_FALSE;
          break;

        // Process message
        case XCB_CLIENT_MESSAGE:

          // Check for close window atom
          if((*(xcb_client_message_event_t*)event).data.data32[0] == close_atom) {
            free(closeReply);
            loop = VK_FALSE;
          }
          break;
      }
      free(event);

    } else {
      draw(ctx);
    }
  }

  // Deallocate structures
  destroy(ctx);
}

// Deallocate surface structures
void destroySurface(Context& ctx) {

  xcb_disconnect(ctx.display);
  ctx.instance.destroySurfaceKHR(ctx.surface);
}
#endif

}