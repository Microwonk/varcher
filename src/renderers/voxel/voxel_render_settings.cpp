#include "voxel_render_settings.h"

static uint32_t scale(FsrScaling scaling, uint32_t dim)
{
    return static_cast<uint32_t>((10.0f / static_cast<uint32_t>(scaling)) * dim);
}

glm::uvec2 VoxelRenderSettings::renderResolution() const
{
#ifdef _WIN32
    if (fsrSettings.enable)
        return glm::uvec2(scale(fsrSettings.scaling, targetResolution.x), scale(fsrSettings.scaling, targetResolution.y));
#endif
    return targetResolution;
}
