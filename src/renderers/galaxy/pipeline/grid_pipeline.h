#pragma once

#include <engine/pipeline/pipeline.h>
#include <engine/pipeline/descriptor_set.h>
#include <engine/resource/shader_module.h>

struct Vertex
{
    glm::vec3 pos;
    glm::vec2 texCoord;
};

struct GridPush {
    glm::vec2 offset;
    // alignas very important, need memory alignment correct for vulkan spec
    alignas(16) glm::mat4 model;
    int numCells;
    float thickness;
    float scroll; // in [1, 2]
};

class GridPipeline : public APipeline {
public:
    std::optional<DescriptorSet> descriptorSet;
    std::optional<vk::PushConstantRange> pushConstantRange;

    static GridPipeline build(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass);

protected:
    GridPipeline(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass) : APipeline(engine, pass) {};

    std::vector<vk::PipelineShaderStageCreateInfo> buildShaderStages() override;

    vk::PipelineVertexInputStateCreateInfo buildVertexInputInfo() override;

    vk::PipelineInputAssemblyStateCreateInfo buildInputAssembly() override;

    vk::PipelineLayoutCreateInfo buildPipelineLayout() override;

    vk::PipelineColorBlendStateCreateInfo buildColorBlendAttachment() override;

private:
    std::optional<ShaderModule> vertexModule;
    std::optional<ShaderModule> fragmentModule;

    std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;

    std::vector<vk::PipelineColorBlendAttachmentState> attachments;
};