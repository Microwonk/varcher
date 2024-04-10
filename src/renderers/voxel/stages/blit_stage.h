#pragma once

#include <memory>
#include "renderers/voxel/voxel_render_stage.h"
#include "renderers/voxel/resource/parameters.h"

#include "engine/resource/buffer.h"
#include "engine/pipeline/render_pass.h"
#include "engine/resource/render_image.h"
#include "engine/pipeline/framebuffer.h"
#include "renderers/voxel/pipeline/blit_pipeline.h"

class BlitStage : public AVoxelRenderStage
{
private:
    BlitOffsets _offsets = {};
    std::unique_ptr<Buffer> _offsetsBuffer;

    std::unique_ptr<BlitPipeline> _pipeline;

public:
    BlitStage(const std::shared_ptr<Engine>& engine, const std::shared_ptr<VoxelRenderSettings>& settings, const RenderPass& renderPass);

    void record(const vk::CommandBuffer& cmd, uint32_t flightFrame, const RenderImage& image, const Framebuffer& windowFramebuffer, const RenderPass& windowRenderPass, const std::function<void(const vk::CommandBuffer&)> uiStage);
};
