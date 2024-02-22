#ifndef MAIN_H_
#define MAIN_H_

#define NUM_VERTICES 256
#define NUM_SPHERES 4

#include <cmath>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Context.hpp"
#include "DeviceUtils.hpp"
#include "CommandUtils.hpp"
#include "ErrorUtils.hpp"
#include "PipelineUtils.hpp"
#include "RenderPassUtils.hpp"
#include "FramebufferUtils.hpp"
#include "ResourceUtils.hpp"
#include "SurfaceUtils.hpp"
#include "SwapchainUtils.hpp"
#include "SyncUtils.hpp"

// Structure for all draw data
typedef struct DrawData_t {
  glm::vec3 vertices[NUM_VERTICES];
  vk::DrawIndirectCommand indirectData;
} DrawData;

// Structure containing push constant data
typedef struct PushData_t {
  glm::mat4 mvpMatrix;
  glm::vec4 color;
} PushData;

#endif /* MAIN_H_ */