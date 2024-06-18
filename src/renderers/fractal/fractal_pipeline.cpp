#include "fractal_pipeline.h"
#include "parameters.h"

std::vector<vk::PipelineShaderStageCreateInfo> FractalPipeline::buildShaderStages() {
    vertexModule = ShaderModule(engine, "../shader/rayMarch.vert.spv", vk::ShaderStageFlagBits::eVertex);
    fragmentModule = ShaderModule(engine, "../shader/rayMarch.frag.spv", vk::ShaderStageFlagBits::eFragment);

    pipelineDeletionQueue.push_group([=]() {
        vertexModule->destroy();
        fragmentModule->destroy();
    });

    return
            {
                    vertexModule->buildStageCreateInfo(),
                    fragmentModule->buildStageCreateInfo()
            };
}

FractalPipeline FractalPipeline::build(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass) {
    FractalPipeline pipeline(engine, pass);
    pipeline.buildAll();
    return pipeline;
}

vk::PipelineLayoutCreateInfo FractalPipeline::buildPipelineLayout() {
    // Shader uniforms
    auto desc = DescriptorSetBuilder(engine)
            .buffer(0, vk::ShaderStageFlagBits::eFragment,
                    vk::DescriptorType::eUniformBuffer) // Binding 0 for UBO (ShaderSettings)
            .build("Fractal Descriptor Set");
    // Store descriptor set for later use
    descriptorSet = desc;
    // Remember to destroy descriptor set when no longer needed
    pushDeletor([=](const std::shared_ptr<Engine> &) {
        desc.destroy();
    });

    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet->layout;

    return layoutInfo;
}

vk::PipelineVertexInputStateCreateInfo FractalPipeline::buildVertexInputInfo() {
    vk::VertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(FractalVertex);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;

    bindingDescriptions.push_back(bindingDescription);

    vk::VertexInputAttributeDescription vertPosAttrib{};
    vertPosAttrib.binding = 0;
    vertPosAttrib.location = 0;
    vertPosAttrib.format = vk::Format::eR32G32B32Sfloat;
    vertPosAttrib.offset = offsetof(FractalVertex, position);

    vk::VertexInputAttributeDescription vertTexCoordAttrib{};
    vertTexCoordAttrib.binding = 0;
    vertTexCoordAttrib.location = 1;
    vertTexCoordAttrib.format = vk::Format::eR32G32Sfloat;
    vertTexCoordAttrib.offset = offsetof(FractalVertex, uv);

    attributeDescriptions.push_back(vertPosAttrib);
    attributeDescriptions.push_back(vertTexCoordAttrib);

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    return vertexInputInfo;
}

vk::PipelineInputAssemblyStateCreateInfo FractalPipeline::buildInputAssembly() {
    // Triangle list with no restart
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyInfo.primitiveRestartEnable = false;
    return inputAssemblyInfo;
}

vk::PipelineRasterizationStateCreateInfo FractalPipeline::buildRasterizer() {
    vk::PipelineRasterizationStateCreateInfo rasterizer;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = vk::PolygonMode::eFill;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = vk::CullModeFlagBits::eNone;
    rasterizer.frontFace = vk::FrontFace::eClockwise;
    rasterizer.depthBiasEnable = VK_FALSE;
    return rasterizer;
}

vk::PipelineMultisampleStateCreateInfo FractalPipeline::buildMultisampling() {
    vk::PipelineMultisampleStateCreateInfo multisampling;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
    multisampling.pSampleMask = &sampleMask;
    return multisampling;
}

vk::PipelineColorBlendStateCreateInfo FractalPipeline::buildColorBlendAttachment() {
    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                          vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
    colorBlendAttachment.blendEnable = VK_FALSE;

    vk::PipelineColorBlendStateCreateInfo colorBlending;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = vk::LogicOp::eCopy;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;
    return colorBlending;
}
