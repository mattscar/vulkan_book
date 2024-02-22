#ifndef SURFACE_UTILS_H_
#define SURFACE_UTILS_H_

#include <iostream>

#include <vulkan/vulkan.hpp>

#include "DeviceUtils.hpp"

// Functions declared in main.c 
extern void draw(Context& ctx);
extern void destroy(Context& ctx);
extern void resizeWindow(Context& ctx);

void createSurface(Context& ctx);
void displaySurface(Context& ctx);
void destroySurface(Context& ctx);

#endif // SURFACE_UTILS_H_ 