#include "voxel_settings_gui.h"

#include "voxel_render_settings.h"
#include <imgui.h>
#include <imgui_stdlib.h>
#include <fmt/format.h>
#include <nfd.h>
#include "voxel_renderer.h"
#include <filesystem>

const std::vector<FsrScaling> scalingOptions = {
    FsrScaling::NONE,
    FsrScaling::QUALITY,
    FsrScaling::BALANCED,
    FsrScaling::PERFORMANCE,
    FsrScaling::ULTRA_PERFORMANCE
};

static std::string scalingName(FsrScaling scaling)
{
    switch (scaling)
    {
        case FsrScaling::NONE:
            return "None (1.0x)";
        case FsrScaling::QUALITY:
            return "Quality (1.5x)";
        case FsrScaling::BALANCED:
            return "Balanced (1.7x)";
        case FsrScaling::PERFORMANCE:
            return "Performance (2x)";
        case FsrScaling::ULTRA_PERFORMANCE:
            return "Ultra Performance (3x)";
        default:
            return "Invalid";
    }
}

const std::vector<glm::uvec2> resolutionOptions = {
    glm::uvec2(3840, 2160),
    glm::uvec2(2560, 1440),
    glm::uvec2(1920, 1080),
    glm::uvec2(1280, 720),
    glm::uvec2(854, 480),
    glm::uvec2(640, 360),
    glm::uvec2(430, 240),
    glm::uvec2(256, 144)
};

static std::string resolutionName(glm::uvec2 resolution)
{
    return fmt::format("{}x{}", resolution.x, resolution.y);
}

RecreationEventFlags VoxelSettingsGui::draw(const std::shared_ptr<VoxelRenderSettings>& settings)
{
    RecreationEventFlags flags;

    ImGui::Begin("Render Settings");

    if (ImGui::CollapsingHeader("Resolution", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::BeginCombo("Target Resolution", resolutionName(settings->targetResolution).c_str()))
        {
            for (const glm::uvec2 resolution : resolutionOptions)
            {
                if (ImGui::Selectable(resolutionName(resolution).c_str(), settings->targetResolution == resolution))
                {
                    settings->targetResolution = resolution;
                    flags |= RecreationEventFlags::TARGET_RESIZE;
                    flags |= RecreationEventFlags::RENDER_RESIZE;
                }

                if (settings->targetResolution == resolution)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
#ifdef _WIN32
        if (ImGui::Checkbox("Enable FSR", &settings->fsrSettings.enable))
        {
            flags |= RecreationEventFlags::RENDER_RESIZE;
        }

        if (ImGui::BeginCombo("FSR Quality", scalingName(settings->fsrSettings.scaling).c_str()))
        {
            for (const FsrScaling scaling: scalingOptions)
            {
                if (ImGui::Selectable(scalingName(scaling).c_str(), settings->fsrSettings.scaling == scaling))
                {
                    settings->fsrSettings.scaling = scaling;
                    flags |= RecreationEventFlags::RENDER_RESIZE;
                }

                if (settings->fsrSettings.scaling == scaling)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
#endif
    }


    if (ImGui::CollapsingHeader("Ambient Occlusion", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderInt("Occlusion Samples", &settings->occlusionSettings.numSamples, 0, 16);
        ImGui::SliderFloat("Ambient Intensity", &settings->occlusionSettings.intensity, 0.0f, 5.0f);
    }

    if (ImGui::CollapsingHeader("Directional Light"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        ImGui::SliderFloat3("Light Direction", reinterpret_cast<float*>(&settings->lightSettings.direction), -1.0f, 1.0f);
        ImGui::ColorEdit4("Light Color", reinterpret_cast<float*>(&settings->lightSettings.color), ImGuiColorEditFlags_HDR);
        ImGui::SliderFloat("Light Intensity", &settings->lightSettings.intensity, 0.0f, 5.0f);
        settings->lightSettings.direction = glm::normalize(settings->lightSettings.direction);
    }

    if (ImGui::CollapsingHeader("Scene"), ImGuiTreeNodeFlags_DefaultOpen)
    {
        ImGui::LabelText("Scene", "%s", settings->voxPath.c_str());
        if (ImGui::Button("Pick Scene"))
        {
            std::string absVox = std::filesystem::absolute(std::filesystem::relative(settings->voxPath)).string();
            nfdchar_t* outPath = nullptr;
            nfdresult_t result = NFD_OpenDialog("vox", absVox.c_str(), &outPath);

            if (result == NFD_OKAY)
            {
                settings->voxPath = outPath;
                free(outPath);
                flags |= RecreationEventFlags::SCENE_PATH;
            }
        }
        ImGui::LabelText("Skybox", "%s", settings->skyboxPath.c_str());
        if (ImGui::Button("Pick Skybox"))
        {
            std::string absSkybox = std::filesystem::absolute(std::filesystem::relative(settings->skyboxPath)).string();
            nfdchar_t* outPath = nullptr;
            nfdresult_t result = NFD_OpenDialog("hdr", absSkybox.c_str(), &outPath);

            if (result == NFD_OKAY)
            {
                settings->skyboxPath = outPath;
                free(outPath);
                flags |= RecreationEventFlags::SCENE_PATH;
            }
        }
    }

    ImGui::End();

    return flags;
}
