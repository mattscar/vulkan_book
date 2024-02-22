#ifndef SURFACE_UTILS_H_
#define SURFACE_UTILS_H_

#include "Context.hpp"
#include "ErrorUtils.hpp"

// Functions declared in main.c 
extern void draw(Context& ctx);
extern void destroy(Context& ctx);
extern void resizeWindow(Context& ctx);

namespace surface_utils {

void createSurface(Context& ctx);

void displaySurface(Context& ctx);

void destroySurface(Context& ctx);

}

#endif // SURFACE_UTILS_H_ 