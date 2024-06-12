#include <renderers/galaxy/resource/parameters.h>
#include "grid_pipeline.h"

GridPipeline GridPipeline::build(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass) {
    GridPipeline pipeline(engine, pass);
    pipeline.buildAll();
    return pipeline;
}

std::vector<vk::PipelineShaderStageCreateInfo> GridPipeline::buildShaderStages() {
    vertexModule = ShaderModule(engine, "../shader/grid.vert.spv", vk::ShaderStageFlagBits::eVertex);
    fragmentModule = ShaderModule(engine, "../shader/grid.frag.spv", vk::ShaderStageFlagBits::eFragment);

    pipelineDeletionQueue.push_group([=]() {
        vertexModule->destroy();
        fragmentModule->destroy();
    });

    return {
            vertexModule->buildStageCreateInfo(),
            fragmentModule->buildStageCreateInfo()
    };
}

vk::PipelineInputAssemblyStateCreateInfo GridPipeline::buildInputAssembly() {
    // Triangle list with no restart
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyInfo.primitiveRestartEnable = false;
    return inputAssemblyInfo;
}

vk::PipelineLayoutCreateInfo GridPipeline::buildPipelineLayout() {
    // Shader uniforms
    auto gridDescriptorSet = DescriptorSetBuilder(engine)
            .buffer(0, vk::ShaderStageFlagBits::eVertex, vk::DescriptorType::eUniformBuffer) // Binding 0 for Camera uniform buffer
            .build("Grid Descriptor Set");
    // Store descriptor set for later use
    descriptorSet = gridDescriptorSet;
    // Remember to destroy descriptor set when no longer needed
    pushDeletor([=](const std::shared_ptr<Engine> &) {
        gridDescriptorSet.destroy();
    });

    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = {};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet->layout;

    return layoutInfo;
}

vk::PipelineVertexInputStateCreateInfo GridPipeline::buildVertexInputInfo() {
    vk::VertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;

    bindingDescriptions.push_back(bindingDescription);

    vk::VertexInputAttributeDescription vertPosAttrib {};
    vertPosAttrib.binding = 0;
    vertPosAttrib.location = 0;
    vertPosAttrib.format = vk::Format::eR32G32B32Sfloat;
    vertPosAttrib.offset = offsetof(Vertex, pos);

    vk::VertexInputAttributeDescription vertTexCoordAttrib {};
    vertTexCoordAttrib.binding = 0;
    vertTexCoordAttrib.location = 1;
    vertTexCoordAttrib.format = vk::Format::eR32G32Sfloat;
    vertTexCoordAttrib.offset = offsetof(Vertex, color);

    attributeDescriptions.push_back(vertPosAttrib);
    attributeDescriptions.push_back(vertTexCoordAttrib);

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexAttributeDescriptionCount = 2;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();

    return vertexInputInfo;
}

//vk::PipelineRasterizationStateCreateInfo GridPipeline::buildRasterizer() {
//    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo = {};
//
//    rasterizationStateCreateInfo.depthClampEnable = false;
//    rasterizationStateCreateInfo.rasterizerDiscardEnable = false;
//    rasterizationStateCreateInfo.polygonMode = vk::PolygonMode::eFill;
//    rasterizationStateCreateInfo.cullMode = vk::CullModeFlagBits::eNone;
//    rasterizationStateCreateInfo.frontFace = vk::FrontFace::eCounterClockwise;
//    rasterizationStateCreateInfo.depthBiasEnable = false;
//    rasterizationStateCreateInfo.depthBiasClamp = 0.f;
//    rasterizationStateCreateInfo.depthBiasConstantFactor = 0.f;
//    rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.f;
//
//    return rasterizationStateCreateInfo;
//}
//
//vk::PipelineColorBlendStateCreateInfo GridPipeline::buildColorBlendAttachment() {
//    vk::PipelineColorBlendStateCreateInfo colorBlendState {};
//    colorBlendState.logicOpEnable = false;
//    colorBlendState.logicOp = vk::LogicOp::eCopy;
//    colorBlendState.blendConstants = std::array<float, 4>{0.f, 0.f, 0.f, 0.f};
//
//    return colorBlendState;
//}
//
//vk::PipelineDepthStencilStateCreateInfo GridPipeline::buildDepthStencil() {
//    vk::PipelineDepthStencilStateCreateInfo depthStencilState {};
//    depthStencilState.depthTestEnable = true;
//    depthStencilState.depthWriteEnable = true;
//    depthStencilState.depthCompareOp = vk::CompareOp::eLess;
//    depthStencilState.depthBoundsTestEnable = false;
//    depthStencilState.stencilTestEnable = false;
//    depthStencilState.minDepthBounds = 0.f;
//    depthStencilState.maxDepthBounds = 1.f;
//
//    return depthStencilState;
//}
//
//vk::PipelineMultisampleStateCreateInfo GridPipeline::buildMultisampling() {
//    vk::PipelineMultisampleStateCreateInfo multisampleState {};
//
//    multisampleState.rasterizationSamples = vk::SampleCountFlagBits::e1;
//    multisampleState.sampleShadingEnable = false;
//    multisampleState.minSampleShading = 1.f;
//    multisampleState.pSampleMask = nullptr;
//    multisampleState.alphaToCoverageEnable = false;
//    multisampleState.alphaToOneEnable = false;
//
//    return multisampleState;
//}