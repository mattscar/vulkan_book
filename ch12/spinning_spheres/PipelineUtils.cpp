#include "PipelineUtils.hpp"

namespace pipeline_utils {

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

  /* Define push constant range */
  vk::PushConstantRange constRange = 
    vk::PushConstantRange()
    .setOffset(0)
    .setSize(16 * sizeof(float) + 4 * sizeof(float));  
  
  // Pipeline layout data
  vk::PipelineLayoutCreateInfo pipelineLayoutInfo = vk::PipelineLayoutCreateInfo()
    .setSetLayoutCount(0)
    .setPSetLayouts(nullptr)
    .setPushConstantRangeCount(1)
    .setPPushConstantRanges(&constRange);

  // Create the pipeline layout
  auto layoutRes = ctx.device.createPipelineLayout(pipelineLayoutInfo);
  ck(layoutRes.result);
  ctx.pipelineLayout = layoutRes.value;

  // Create vertex shader module
  vk::ShaderModuleCreateInfo vertexShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(vertexShaderCode.size())
    .setPCode((uint32_t*)vertexShaderCode.data());
  auto vertRes = ctx.device.createShaderModule(vertexShaderCreateInfo);
  ck(vertRes.result);
  ctx.vertexModule = vertRes.value;

  // Create fragment shader module
  vk::ShaderModuleCreateInfo fragmentShaderCreateInfo =
    vk::ShaderModuleCreateInfo()
    .setCodeSize(fragmentShaderCode.size())
    .setPCode((uint32_t*)fragmentShaderCode.data());
  auto fragRes = ctx.device.createShaderModule(fragmentShaderCreateInfo);
  ck(fragRes.result);
  ctx.fragmentModule = fragRes.value;

  // Shader configuration data
  vk::PipelineShaderStageCreateInfo shaderStageInfos[2] = {

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
  };

  vk::VertexInputBindingDescription bindingInfo = vk::VertexInputBindingDescription()
    .setBinding(0)
    .setStride(3 * sizeof(float))
    .setInputRate(vk::VertexInputRate::eVertex);

  vk::VertexInputAttributeDescription attributeInfos[1] = {

    // Vertex coordinates
    vk::VertexInputAttributeDescription()
      .setBinding(0)
      .setLocation(0)
      .setFormat(vk::Format::eR32G32B32Sfloat)
      .setOffset(0)
  };

  vk::PipelineVertexInputStateCreateInfo vertexInputStateCreateInfo =
    vk::PipelineVertexInputStateCreateInfo()
    .setVertexBindingDescriptionCount(1)
    .setPVertexBindingDescriptions(&bindingInfo)
    .setVertexAttributeDescriptionCount(1)
    .setPVertexAttributeDescriptions(attributeInfos);

  vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo =
    vk::PipelineInputAssemblyStateCreateInfo()
    .setTopology(vk::PrimitiveTopology::eLineStrip)
    .setPrimitiveRestartEnable(VK_FALSE);

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
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA);

  vk::PipelineColorBlendStateCreateInfo colorBlendStateCreateInfo =
    vk::PipelineColorBlendStateCreateInfo()
    .setLogicOpEnable(VK_FALSE)
    .setLogicOp(vk::LogicOp::eCopy)
    .setAttachmentCount(1)
    .setPAttachments(&colorBlendAttachmentState)
    .setBlendConstants({ 0.0f, 0.0f, 0.0f, 0.0f });

  vk::DynamicState stateArray[2] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};  
  vk::PipelineDynamicStateCreateInfo dynamicStateCreateInfo = 
    vk::PipelineDynamicStateCreateInfo()
    .setDynamicStateCount(2)
    .setPDynamicStates(stateArray);    
    
  // Create the info that configures the graphic pipeline
  vk::PipelineViewportStateCreateInfo viewportStateCreateInfo;
  vk::GraphicsPipelineCreateInfo pipelineCreateInfo =
    vk::GraphicsPipelineCreateInfo()
    .setStageCount(2)
    .setPStages(shaderStageInfos)
    .setPVertexInputState(&vertexInputStateCreateInfo)
    .setPInputAssemblyState(&inputAssemblyStateCreateInfo)
    .setPTessellationState(nullptr)
    .setPViewportState(&viewportStateCreateInfo)
    .setPRasterizationState(&rasterizationStateCreateInfo)
    .setPMultisampleState(&multisampleStateCreateInfo)
    .setPDepthStencilState(nullptr)
    .setPColorBlendState(&colorBlendStateCreateInfo)
    .setPDynamicState(&dynamicStateCreateInfo)
    .setLayout(ctx.pipelineLayout)
    .setRenderPass(ctx.renderPass)
    .setSubpass(0)
    .setBasePipelineHandle(nullptr)
    .setBasePipelineIndex(-1);

  // Create the graphics pipeline
  auto pipeRes = ctx.device.createGraphicsPipelines(nullptr,
    std::array<vk::GraphicsPipelineCreateInfo, 1>({pipelineCreateInfo}));
  ck(pipeRes.result);
  ctx.pipeline = pipeRes.value[0];
}

// Deallocate resources
void destroyPipeline(Context& ctx) {

  ctx.device.destroyPipeline(ctx.pipeline);
  ctx.device.destroyPipelineLayout(ctx.pipelineLayout);
  ctx.device.destroyShaderModule(ctx.fragmentModule);
  ctx.device.destroyShaderModule(ctx.vertexModule);
}

}