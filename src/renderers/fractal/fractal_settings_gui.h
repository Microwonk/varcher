#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include "engine/util/recreation_queue.h"
#include "parameters.h"

namespace FractalSettingsGui
{
    static int previousFractalType = 4;
    extern RecreationEventFlags draw(std::shared_ptr<ShaderSettings> &settings);
}