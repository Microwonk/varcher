#pragma once

#include <memory>
#include "engine/util/recreation_queue.h"

class VoxelRenderSettings;
class VoxelRenderer;

namespace VoxelSettingsGui
{
    extern RecreationEventFlags draw(const std::shared_ptr<VoxelRenderSettings>& settings);
}
