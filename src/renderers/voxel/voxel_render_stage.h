#pragma once

#include "engine/resource.h"
#include "voxel_render_settings.h"

class AVoxelRenderStage : public AResource
{
protected:
    std::shared_ptr<VoxelRenderSettings> _settings;

public:
    AVoxelRenderStage(const std::shared_ptr<Engine>& engine, const std::shared_ptr<VoxelRenderSettings>& settings)
        : AResource(engine), _settings(settings) {}
};
