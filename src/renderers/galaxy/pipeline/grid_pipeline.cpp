#include "grid_pipeline.h"

struct GridPushVert {
    glm::mat4 model;
};

struct GridPushFrag {
    glm::vec2 offset;
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
    // push constants
    v_pushConstantRange->offset = 0;
    v_pushConstantRange->size = sizeof(GridPushVert);
    v_pushConstantRange->stageFlags = vk::ShaderStageFlagBits::eVertex;

    f_pushConstantRange->offset = sizeof(GridPushVert);
    f_pushConstantRange->size = sizeof(GridPushFrag);
    f_pushConstantRange->stageFlags = vk::ShaderStageFlagBits::eFragment;

    std::array<vk::PushConstantRange, 2> pushConstantRanges = {
            {
                    // Vertex shader push constant range
                    v_pushConstantRange.value(),
                    // Fragment shader push constant range
                    f_pushConstantRange.value()
            }
    };

    // Shader uniforms
    auto gridDescriptorSet = DescriptorSetBuilder(engine)
            .buffer(0, vk::ShaderStageFlagBits::eVertex, vk::DescriptorType::eUniformBuffer) // Binding 0 for Camera uniform buffer
            .build("Grid Descriptor Set");

    // Set up push constant range
    vk::PipelineLayoutCreateInfo layoutInfo;
    layoutInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
    layoutInfo.pPushConstantRanges = pushConstantRanges.data(); // also add f_pushConstantRange.value()

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
