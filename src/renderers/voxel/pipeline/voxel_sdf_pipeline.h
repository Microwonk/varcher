#pragma once

#include <memory>
#include <optional>
#include "engine/pipeline/pipeline.h"
#include "engine/resource/shader_module.h"
#include "engine/util/resource_ring.h"
#include "engine/pipeline/descriptor_set.h"

class VoxelSDFPipeline : public APipeline
{
public:
    std::optional<DescriptorSet> descriptorSet;

private:
    std::optional<ShaderModule> vertexModule;
    std::optional<ShaderModule> fragmentModule;

    std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;

    std::optional<vk::PushConstantRange> pushConstantRange;

protected:
    VoxelSDFPipeline(const std::shared_ptr<Engine>& engine, const vk::RenderPass& pass) : APipeline(engine, pass) {};

public:
    static VoxelSDFPipeline build(const std::shared_ptr<Engine>& engine, const vk::RenderPass& pass);

protected:
    std::vector<vk::PipelineShaderStageCreateInfo> buildShaderStages() override;
    vk::PipelineVertexInputStateCreateInfo buildVertexInputInfo() override;
    vk::PipelineInputAssemblyStateCreateInfo buildInputAssembly() override;
    vk::PipelineLayoutCreateInfo buildPipelineLayout() override;
    vk::PipelineColorBlendStateCreateInfo buildColorBlendAttachment() override;
};
