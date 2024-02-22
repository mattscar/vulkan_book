#ifndef SURFACE_UTILS_H_
#define SURFACE_UTILS_H_

#include <xcb/xcb.h>

#include "context.h"
#include "device_utils.h"

/* Functions declared in main.c */
extern void draw(Context* ctx);
extern void destroy(Context* ctx);

void create_surface(Context* ctx);
void display_surface(Context* ctx);
void destroy_surface(Context* ctx);

#endif /* SURFACE_UTILS_H_ */