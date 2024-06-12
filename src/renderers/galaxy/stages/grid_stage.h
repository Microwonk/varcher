#pragma once

#include <engine/resource.h>
#include <renderers/galaxy/pipeline/grid_pipeline.h>
#include <engine/resource/render_image.h>
#include <engine/pipeline/framebuffer.h>
#include <engine/pipeline/render_pass.h>
#include <engine/engine.h>
#include <renderers/common/camera_controller.h>
#include "engine/resource/buffer.h"
#include "renderers/galaxy/resource/parameters.h"

class GridStage : public AResource {
private:
    std::unique_ptr<GridPipeline> _pipeline;

    ViewUniforms _camera = {};
    std::unique_ptr<Buffer> _cameraBuffer;

    std::unique_ptr<Buffer> _quadVertBuffer;
    std::unique_ptr<Buffer> _quadIndexBuffer;

public:
    GridStage(const std::shared_ptr<Engine>& engine, const RenderPass& renderPass);

    void record(const vk::CommandBuffer &cmd, uint32_t flightFrame,
                const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                const std::function<void(const vk::CommandBuffer &)>& uiStage,
                const std::shared_ptr<CameraController>& params);

    [[nodiscard]] const vk::PipelineLayout &getPipelineLayout() const;
};