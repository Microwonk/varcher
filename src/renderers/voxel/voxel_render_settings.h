#pragma once

#include "engine/util/recreation_queue.h"
#include <glm/glm.hpp>

enum class FsrScaling : uint32_t
{
    NONE = 10,
    QUALITY = 15,
    BALANCED = 17,
    PERFORMANCE = 20,
    ULTRA_PERFORMANCE = 30
};

struct FsrSettings
{
    bool enable = false;
    FsrScaling scaling = FsrScaling::BALANCED;
};

struct AmbientOcclusionSettings
{
    int numSamples = 0;
    float intensity = 1.0f;
};

struct LightSettings
{
    glm::vec3 direction = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    float intensity = 1.0f;
};

class VoxelRenderSettings
{
public:
    glm::uvec2 targetResolution = { 1920, 1080 };

#ifdef _WIN32
    FsrSettings fsrSettings = {};
#endif
    AmbientOcclusionSettings occlusionSettings = {};
    LightSettings lightSettings = {};

    std::string voxPath = "../resource/floatingcolored.vox";
    std::string skyboxPath = "../resource/rustig_koppie.hdr";

    [[nodiscard]] glm::uvec2 renderResolution() const;
};

