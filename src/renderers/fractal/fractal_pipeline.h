#pragma once

#include "engine/pipeline/pipeline.h"
#include "engine/pipeline/descriptor_set.h"
#include "engine/resource/shader_module.h"

class FractalPipeline : public APipeline {
public:
    std::optional<DescriptorSet> descriptorSet;

private:
    std::optional<ShaderModule> vertexModule;
    std::optional<ShaderModule> fragmentModule;

    std::optional<vk::PushConstantRange> pushConstantRange;

    std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;

    vk::SampleMask sampleMask = 0xFFFFFF;
    vk::PipelineColorBlendAttachmentState colorBlendAttachment;

protected:
    FractalPipeline(const std::shared_ptr<Engine>& engine, const vk::RenderPass& pass) : APipeline(engine, pass) {};

public:
    static FractalPipeline build(const std::shared_ptr<Engine>& engine, const vk::RenderPass& pass);

protected:
    std::vector<vk::PipelineShaderStageCreateInfo> buildShaderStages() override;
    vk::PipelineVertexInputStateCreateInfo buildVertexInputInfo() override;
    vk::PipelineInputAssemblyStateCreateInfo buildInputAssembly() override;
    vk::PipelineLayoutCreateInfo buildPipelineLayout() override;
    vk::PipelineMultisampleStateCreateInfo buildMultisampling() override;
    vk::PipelineColorBlendStateCreateInfo buildColorBlendAttachment() override;
    vk::PipelineRasterizationStateCreateInfo buildRasterizer() override;
};
