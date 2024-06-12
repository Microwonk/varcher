#pragma once

#include <engine/pipeline/pipeline.h>
#include <engine/pipeline/descriptor_set.h>
#include <engine/resource/shader_module.h>

class GridPipeline : public APipeline {
public:
    std::optional<DescriptorSet> descriptorSet;

    static GridPipeline build(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass);

protected:
    GridPipeline(const std::shared_ptr<Engine> &engine, const vk::RenderPass &pass) : APipeline(engine, pass) {};

    std::vector<vk::PipelineShaderStageCreateInfo> buildShaderStages() override;

    vk::PipelineInputAssemblyStateCreateInfo buildInputAssembly() override;

    vk::PipelineLayoutCreateInfo buildPipelineLayout() override;

    vk::PipelineVertexInputStateCreateInfo buildVertexInputInfo() override;

//    vk::PipelineRasterizationStateCreateInfo buildRasterizer() override;
//
//    vk::PipelineColorBlendStateCreateInfo buildColorBlendAttachment() override;
//
//    vk::PipelineDepthStencilStateCreateInfo buildDepthStencil() override;
//
//    vk::PipelineMultisampleStateCreateInfo buildMultisampling() override;

private:
    std::optional<ShaderModule> vertexModule;
    std::optional<ShaderModule> fragmentModule;

    std::vector<vk::VertexInputBindingDescription> bindingDescriptions;
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;

};