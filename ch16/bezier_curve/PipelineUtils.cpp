#include "PipelineUtils.hpp"

std::vector<uint8_t> readFile(const char* filename) {

  // Open file
  std::ifstream file(filename, std::ios::binary);
  file.unsetf(std::ios::skipws);

  // Get file size
  std::streampos size;
  file.seekg(0, std::ios::end);
  size = file.tellg();
  file.seekg(0, std::ios::beg);

  // Create vector
  std::vector<uint8_t> outVec;
  outVec.reserve(size);

  // Read data into vector
  outVec.insert(outVec.begin(),
    std::istream_iterator<uint8_t>(file),
    std::istream_iterator<uint8_t>());

  file.close();
  return outVec;
}

// Create and configure a graphical pipeline
void createPipeline(Context& ctx) {

  // Read shader code from files
  std::vector<uint8_t> vertexShaderCode = readFile("shaders/vert.spv");
  std::vector<uint8_t> fragmentShaderCode = readFile("shaders/frag.spv");
  std::vector<uint8_t> tescShaderCode = readFile("shaders/tesc.spv");
  std::vector<uint8_t> teseShaderCode = readFile("shaders/tese.spv");

  // Define push constant range
  vk::PushConstantRange constRange = vk::PushConstantRange()
    .setStageFlags(vk::ShaderStageFlagBits::eTessellationControl)
    .setOffset(0)
    .setSize(2 * sizeof(float));

  // Pipeline layout data
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo = vk::PipelineLayoutCreateInfo()
    .setSetLayoutCount(0)
    .setPSetLayouts(nullptr)
    .setPushConstantRangeCount(1)
    .setPPushConstantRanges(&constRange);

  // Create the pipeline layout
  ctx.pipelineLayout = ctx.device.createPipelineLayout(pipelineLayoutInfo);

  // Create vertex shader module
  vk::ShaderModuleCreateInfo vertexShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(vertexShaderCode.size())
    .setPCode((uint32_t*)vertexShaderCode.data());
  ctx.vertexModule = ctx.device.createShaderModule(vertexShaderCreateInfo);

  // Create fragment shader module
  vk::ShaderModuleCreateInfo fragmentShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(fragmentShaderCode.size())
    .setPCode((uint32_t*)fragmentShaderCode.data());
  ctx.fragmentModule = ctx.device.createShaderModule(fragmentShaderCreateInfo);

  // Create tessellation control shader module
  vk::ShaderModuleCreateInfo tescShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(tescShaderCode.size())
    .setPCode((uint32_t*)tescShaderCode.data());
  ctx.tescModule = ctx.device.createShaderModule(tescShaderCreateInfo);

  // Create tessellation evaluation shader module
  vk::ShaderModuleCreateInfo teseShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(teseShaderCode.size())
    .setPCode((uint32_t*)teseShaderCode.data());
  ctx.teseModule = ctx.device.createShaderModule(teseShaderCreateInfo);

  // Shader configuration data
  vk::PipelineShaderStageCreateInfo shaderStageInfos[4] = {

    // Vertex shader
    vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eVertex)
      .setModule(ctx.vertexModule)
      .setPName("main"),

    // Fragment shader
    vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eFragment)
      .setModule(ctx.fragmentModule)
      .setPName("main"),

    // Tessellation control shader
    vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eTessellationControl)
      .setModule(ctx.tescModule),

    // Tessellation evaluation shader
    vk::PipelineShaderStageCreateInfo()
      .setStage(vk::ShaderStageFlagBits::eTessellationEvaluation)
      .setModule(ctx.teseModule)
  };

  vk::VertexInputBindingDescription bindingInfo = vk::VertexInputBindingDescription()
    .setBinding(0)
    .setStride(5 * sizeof(float))
    .setInputRate(vk::VertexInputRate::eVertex);

  vk::VertexInputAttributeDescription attributeInfos[2] = {

    // Vertex coordinates
    vk::VertexInputAttributeDescription()
      .setBinding(0)
      .setLocation(0)
      .setFormat(vk::Format::eR32G32Sfloat)
      .setOffset(0),

    // Vertex colors
    vk::VertexInputAttributeDescription()
      .setBinding(0)
      .setLocation(1)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(2 * sizeof(float))
  };

  vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo =
    vk::PipelineVertexInputStateCreateInfo()
    .setVertexBindingDescriptionCount(1)
    .setPVertexBindingDescriptions(&bindingInfo)
    .setVertexAttributeDescriptionCount(2)
    .setPVertexAttributeDescriptions(attributeInfos);

  vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo =
    vk::PipelineInputAssemblyStateCreateInfo()
    .setTopology(vk::PrimitiveTopology::eTriangleStrip)
    .setPrimitiveRestartEnable(VK_FALSE);

  vk::PipelineTessellationStateCreateInfo tessellationStateCreateInfo = 
    vk::PipelineTessellationStateCreateInfo()
    .setPatchControlPoints(4);
    
  // Define the pipeline's viewport
  vk::Viewport viewport = vk::Viewport()
    .setX(1.0f * ctx.x)
    .setY(1.0f * ctx.y)
    .setWidth(1.0f * ctx.width)
    .setHeight(1.0f * ctx.height)
    .setMinDepth(0.0f)
    .setMaxDepth(1.0f);

  // Define a scissor rectangle
  vk::Rect2D scissor = vk::Rect2D()
    .setOffset(vk::Offset2D(ctx.x, ctx.y))
    .setExtent(vk::Extent2D(ctx.width, ctx.height));

  vk::PipelineViewportStateCreateInfo viewportStateCreateInfo =
    vk::PipelineViewportStateCreateInfo()
    .setViewportCount(1)
    .setPViewports(&viewport)
    .setScissorCount(1)
    .setPScissors(&scissor);

  vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo =
    vk::PipelineRasterizationStateCreateInfo()
    .setDepthClampEnable(VK_FALSE)
    .setRasterizerDiscardEnable(VK_FALSE)
    .setPolygonMode(vk::PolygonMode::eLine)
    .setCullMode(vk::CullModeFlagBits::eNone)
    .setFrontFace(vk::FrontFace::eCounterClockwise)
    .setDepthBiasEnable(VK_FALSE)
    .setDepthBiasConstantFactor(0.0f)
    .setDepthBiasClamp(0.0f)
    .setDepthBiasSlopeFactor(0.0f)
    .setLineWidth(3.0f);

  vk::PipelineMultisampleStateCreateInfo multisampleStateCreateInfo =
    vk::PipelineMultisampleStateCreateInfo()
    .setRasterizationSamples(vk::SampleCountFlagBits::e1)
    .setSampleShadingEnable(VK_FALSE)
    .setMinSampleShading(1.0f)
    .setPSampleMask(nullptr)
    .setAlphaToCoverageEnable(VK_FALSE)
    .setAlphaToOneEnable(VK_FALSE);

  vk::PipelineColorBlendAttachmentState colorBlendAttachmentState =
    vk::PipelineColorBlendAttachmentState()
    .setBlendEnable(VK_FALSE)
    .setSrcColorBlendFactor(vk::BlendFactor::eOne)
    .setDstColorBlendFactor(vk::BlendFactor::eZero)
    .setColorBlendOp(vk::BlendOp::eAdd)
    .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
    .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
    .setAlphaBlendOp(vk::BlendOp::eAdd)
    .setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo =
    vk::PipelineColorBlendStateCreateInfo()
    .setLogicOpEnable(VK_FALSE)
    .setLogicOp(vk::LogicOp::eCopy)
    .setAttachmentCount(1)
    .setPAttachments(&colorBlendAttachmentState)
    .setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f });

  // Create the info that configures the graphic pipeline
  vk::GraphicsPipelineCreateInfo pipelineCreateInfo =
    vk::GraphicsPipelineCreateInfo()
    .setStageCount(4)
    .setPStages(shaderStageInfos)
    .setPVertexInputState(&vertexInputStateCreateInfo)
    .setPInputAssemblyState(&inputAssemblyStateCreateInfo)
    .setPTessellationState(&tessellationStateCreateInfo)
    .setPViewportState(&viewportStateCreateInfo)
    .setPRasterizationState(&rasterizationStateCreateInfo)
    .setPMultisampleState(&multisampleStateCreateInfo)
    .setPDepthStencilState(nullptr)
    .setPColorBlendState(&colorBlendStateCreateInfo)
    .setPDynamicState(nullptr)
    .setLayout(ctx.pipelineLayout)
    .setRenderPass(ctx.renderPass)
    .setSubpass(0)
    .setBasePipelineHandle(nullptr)
    .setBasePipelineIndex(-1);

  // Create the graphics pipeline
  ctx.pipeline = ctx.device.createGraphicsPipelines(nullptr,
    std::array<vk::GraphicsPipelineCreateInfo, 1>({pipelineCreateInfo}))[0];

}

// Deallocate resources
void destroyPipeline(Context& ctx) {

  ctx.device.destroyPipeline(ctx.pipeline);
  ctx.device.destroyPipelineLayout(ctx.pipelineLayout);
  ctx.device.destroyShaderModule(ctx.teseModule);
  ctx.device.destroyShaderModule(ctx.tescModule);
  ctx.device.destroyShaderModule(ctx.fragmentModule);
  ctx.device.destroyShaderModule(ctx.vertexModule);
}