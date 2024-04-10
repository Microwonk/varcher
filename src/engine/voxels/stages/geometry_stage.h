#pragma once

#include <memory>
#include <array>
#include <vulkan/vulkan.hpp>
#include "engine/voxels/voxel_render_stage.h"
#include "engine/util/resource_ring.h"
#include "engine/voxels/resource/parameters.h"

#include "engine/resource/buffer.h"
#include "engine/resource/render_image.h"
#include "engine/pipeline/render_pass.h"
#include "engine/pipeline/framebuffer.h"
#include "engine/voxels/pipeline/voxel_sdf_pipeline.h"
#include "engine/voxels/resource/voxel_scene.h"
#include "engine/resource/texture_2d.h"
#include "engine/voxels/voxel_render_settings.h"

struct GeometryBuffer
{
    std::reference_wrapper<const RenderImage> color;
    std::reference_wrapper<const RenderImage> depth;
    std::reference_wrapper<const RenderImage> motion;
    std::reference_wrapper<const RenderImage> mask;
    std::reference_wrapper<const RenderImage> normal;
    std::reference_wrapper<const RenderImage> position;
};

class GeometryStage : public AVoxelRenderStage
{
private:
    std::shared_ptr<VoxelScene> _scene;

    VolumeParameters _parameters = {};
    std::unique_ptr<Buffer> _parametersBuffer;

    std::unique_ptr<RenderImage> _colorTarget;
    std::unique_ptr<RenderImage> _depthTarget;
    std::unique_ptr<RenderImage> _motionTarget;
    std::unique_ptr<RenderImage> _maskTarget;
    std::unique_ptr<RenderImage> _normalTarget;
    ResourceRing<RenderImage> _positionTargets;

    std::unique_ptr<RenderPass> _renderPass;

    ResourceRing<Framebuffer> _framebuffers;

    std::unique_ptr<VoxelSDFPipeline> _pipeline;

public:
    GeometryStage(const std::shared_ptr<Engine>& engine, const std::shared_ptr<VoxelRenderSettings>& settings, const std::shared_ptr<VoxelScene>& scene, const std::shared_ptr<Texture2D>& noise);

    GeometryBuffer record(const vk::CommandBuffer& cmd, uint32_t flightFrame);

    [[nodiscard]] const vk::PipelineLayout& getPipelineLayout() const;
};
