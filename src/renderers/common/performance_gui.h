#pragma once

#include <engine/util/recreation_queue.h>
#include "performance_settings.h"

namespace PerformanceGui
{
    extern RecreationEventFlags draw(float delta, const std::shared_ptr<PerformanceSettings>& settings);
}
