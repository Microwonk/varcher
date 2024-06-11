#include "grid_pipeline.h"

struct GridPush {
    glm::vec2 offset;
    // alignas very important, need memory alignment correct for vulkan spec
    alignas(16) glm::mat4 model;
    int numCells;
    float thickness;
    float scroll; // in [1, 2]
};

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

vk::PipelineVertexInputStateCreateInfo GridPipeline::buildVertexInputInfo() {
    bindingDescriptions.clear();
    attributeDescriptions.clear();

    vk::VertexInputBindingDescription bindingDescription = {};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = vk::VertexInputRate::eVertex;
    bindingDescriptions.push_back(bindingDescription);

    vk::VertexInputAttributeDescription vertPositionAttrib = {};
    vertPositionAttrib.binding = 0;
    vertPositionAttrib.location = 0;
    vertPositionAttrib.format = vk::Format::eR32G32B32Sfloat;
    vertPositionAttrib.offset = offsetof(Vertex, pos);
    attributeDescriptions.push_back(vertPositionAttrib);

    vk::VertexInputAttributeDescription vertTexCoordAttrib = {};
    vertTexCoordAttrib.binding = 0;
    vertTexCoordAttrib.location = 1;
    vertTexCoordAttrib.format = vk::Format::eR32G32Sfloat;
    vertTexCoordAttrib.offset = offsetof(Vertex, texCoord);
    attributeDescriptions.push_back(vertTexCoordAttrib);

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    return vertexInputInfo;
}


vk::PipelineInputAssemblyStateCreateInfo GridPipeline::buildInputAssembly() {
    // Triangle list with no restart
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;
    inputAssemblyInfo.primitiveRestartEnable = false;
    return inputAssemblyInfo;
}

vk::PipelineLayoutCreateInfo GridPipeline::buildPipelineLayout() {
    // push constants
    pushConstantRange = vk::PushConstantRange();
    pushConstantRange->offset = 0;
    pushConstantRange->size = sizeof(GridPush);
    pushConstantRange->stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;

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

    // Set up push constant range
    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges = &pushConstantRange.value();
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet->layout;

    return layoutInfo;
}

//vk::PipelineColorBlendStateCreateInfo GridPipeline::buildColorBlendAttachment() {
//    vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
//    colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
//    colorBlendAttachment.blendEnable = false;
//    colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
//    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
//    colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
//    colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
//    colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
//    colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
//
//    vk::PipelineColorBlendStateCreateInfo ret{};
//    ret.attachmentCount = 1;
//    ret.pAttachments = &colorBlendAttachment;
//    return ret;
//}
