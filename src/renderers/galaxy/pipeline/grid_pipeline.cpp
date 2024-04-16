#include "grid_pipeline.h"

struct GridPush {
    glm::mat4 model;
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
    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;

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
    // Screen push constants
    pushConstantRange->offset = 0;
    pushConstantRange->size = sizeof(GridPush); // Assuming Model is the push constant structure
    pushConstantRange->stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;

    // Shader uniforms
    auto gridDescriptorSet = DescriptorSetBuilder(engine)
            .buffer(0, vk::ShaderStageFlagBits::eVertex, vk::DescriptorType::eUniformBuffer) // Binding 0 for Camera uniform buffer
            .buffer(1, vk::ShaderStageFlagBits::eFragment, vk::DescriptorType::eUniformBuffer) // Binding 1 for Params push constant
            .image(2, vk::ShaderStageFlagBits::eFragment) // Binding 2 for image in fragment shader
            .build("Grid Descriptor Set");

    // Set up push constant range
    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges = &pushConstantRange.value();

    // Set up descriptor set layout
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &gridDescriptorSet.layout;

    // Store descriptor set for later use
    descriptorSet = gridDescriptorSet;

    // Remember to destroy descriptor set when no longer needed
    pushDeletor([=](const std::shared_ptr<Engine> &) {
        gridDescriptorSet.destroy();
    });

    return layoutInfo;
}
