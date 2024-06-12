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

    // Set up push constant range
    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = vk::StructureType::ePipelineLayoutCreateInfo;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = {};
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet->layout;

    return layoutInfo;
}

vk::PipelineVertexInputStateCreateInfo GridPipeline::buildVertexInputInfo() {
    // no inputs needed
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    return vertexInputInfo;
}
