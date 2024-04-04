#pragma once

#include <memory>
#include "../voxel_render_stage.h"
#include "../resource/parameters.h"

class Buffer;
class BlitPipeline;
class RenderImage;
class Framebuffer;
class RenderPass;

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
