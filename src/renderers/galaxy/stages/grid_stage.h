#pragma once

#include <engine/resource.h>
#include <renderers/galaxy/pipeline/grid_pipeline.h>
#include <engine/resource/render_image.h>
#include <engine/pipeline/framebuffer.h>
#include <engine/pipeline/render_pass.h>

class GridStage : public AResource {
private:
    std::unique_ptr<GridPipeline> _pipeline;

public:
    GridStage(const std::shared_ptr<Engine>& engine, const RenderPass& renderPass);

    void record(const vk::CommandBuffer &cmd, uint32_t flightFrame, const RenderImage &image,
                const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                std::function<void(const vk::CommandBuffer &)> uiStage);

    [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const;
};