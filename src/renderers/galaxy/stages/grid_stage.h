#pragma once

#include <engine/resource.h>
#include <renderers/galaxy/pipeline/grid_pipeline.h>
#include <engine/resource/render_image.h>
#include <engine/pipeline/framebuffer.h>
#include <engine/pipeline/render_pass.h>
#include <engine/engine.h>
#include "engine/resource/buffer.h"
#include "renderers/galaxy/resource/parameters.h"

class GridStage : public AResource {
private:
    std::unique_ptr<GridPipeline> _pipeline;

    Camera _camera = {};
    std::unique_ptr<Buffer> _cameraBuffer;

public:
    GridStage(const std::shared_ptr<Engine>& engine, const RenderPass& renderPass);

    void record(const vk::CommandBuffer &cmd, uint32_t flightFrame, const RenderImage &image,
                const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                std::function<void(const vk::CommandBuffer &)> uiStage);

    [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const;
};