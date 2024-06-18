#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include "engine/util/deletion_queue.h"
#include "engine/resource.h"

class Engine;

// An abstract Vulkan graphics pipelines.
// Different configurations should be implemented as subclasses of this base class.
class APipeline : public AResource
{
public:
    vk::PipelineLayout layout;

    vk::Pipeline pipeline;
protected:
    DeletionQueue pipelineDeletionQueue;

    vk::RenderPass pass;

    std::vector<vk::DynamicState> _dynamicStates;
    vk::PipelineColorBlendAttachmentState _colorBlendAttachment;

protected:
    APipeline(const std::shared_ptr<Engine>& engine, const vk::RenderPass& pass);

public:
    void buildAll();

protected:
    virtual std::vector<vk::PipelineShaderStageCreateInfo> buildShaderStages() = 0;
    virtual vk::PipelineVertexInputStateCreateInfo buildVertexInputInfo() = 0;
    virtual vk::PipelineInputAssemblyStateCreateInfo buildInputAssembly() = 0;
    virtual vk::PipelineDynamicStateCreateInfo buildDynamicState();
    virtual vk::PipelineViewportStateCreateInfo buildViewport();
    virtual vk::PipelineRasterizationStateCreateInfo buildRasterizer();
    virtual vk::PipelineColorBlendStateCreateInfo buildColorBlendAttachment();
    virtual vk::PipelineDepthStencilStateCreateInfo buildDepthStencil();
    virtual vk::PipelineMultisampleStateCreateInfo buildMultisampling();
    virtual vk::PipelineLayoutCreateInfo buildPipelineLayout();
};
