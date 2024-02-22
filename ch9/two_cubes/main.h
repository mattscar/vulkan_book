#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <vulkan/vulkan.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "device_utils.h"
#include "command_utils.h"
#include "descriptor_utils.h"
#include "pipeline_utils.h"
#include "render_pass_utils.h"
#include "framebuffer_utils.h"
#include "resource_utils.h"
#include "surface_utils.h"
#include "swapchain_utils.h"
#include "sync_utils.h"

#define NUM_VERTICES 24
#define NUM_INDICES 30
#define NUM_INSTANCES 3
#define NUM_DESCRIPTOR_BINDINGS 2

/* Structure containing per-vertex data */
typedef struct Vertex_t {
  glm::vec3 position;
  glm::vec3 color;
  glm::vec3 normal;
} Vertex;

/* Structure containing uniform data for the vertex shader */
typedef struct VertexUniform_t {
  glm::mat4 model_matrix;
  glm::mat4 view_matrix;
  glm::mat4 projection_matrix;
  glm::vec3 light_pos;
  glm::vec3 viewer_pos;
} VertexUniform;

/* Structure containing uniform data for the vertex shader */
typedef struct FragmentUniform_t {
  glm::vec3 light_params[3];
  float shininess;
} FragmentUniform;

/* Structure containing all drawing data */
typedef struct DrawData_t {
  Vertex vertex_data[NUM_VERTICES];
  uint16_t index_data[NUM_INDICES];
  VkDrawIndexedIndirectCommand indirect_data;
  VertexUniform vertex_uniform_data;
  FragmentUniform fragment_uniform_data;
} DrawData;

#endif /* MAIN_H_ */