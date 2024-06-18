#include <imgui.h>
#include "fractal_settings_gui.h"

RecreationEventFlags FractalSettingsGui::draw(std::shared_ptr<ShaderSettings> &settings)
{
    RecreationEventFlags flags;

    ImGui::Begin("Render Settings");

    // ImGuiInputTextFlags floatInput = ImGuiInputTextFlags_EnterReturnsTrue;
    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::InputFloat("fov", &settings->fov, 0.1f, 0.1f, "%.1f");
    }

    if (ImGui::CollapsingHeader("Light")) {
        if (ImGui::Button("Dir light shading")) {
            settings->dirLightShading = std::abs(settings->dirLightShading - 1.0);
        }

        ImGui::ColorEdit3("Dir color", glm::value_ptr(settings->dirLightColor));
        ImGui::InputFloat("Dir intensity", &settings->dirIntensity, 0.01f, 0.01f, "%.3f");

        ImGui::ColorEdit3("Specular color", glm::value_ptr(settings->specularColor));
        ImGui::InputFloat("Specular intensity", &settings->specularIntensity, 0.01f, 0.01f, "%.3f");

        ImGui::ColorEdit3("Background color", glm::value_ptr(settings->backgroundColor));
        ImGui::InputFloat("Shadow sharpness", &settings->shadowSharpness, 0.1f, 0.1f, "%.1f");
    }

    if (ImGui::CollapsingHeader("Effects")) {
        ImGui::ColorEdit3("Glow color", glm::value_ptr(settings->glowColor));
        ImGui::InputFloat("Glow intensity", &settings->glowIntensity, 0.001f, 0.001f, "%.3f");

        ImGui::ColorEdit3("AO color", glm::value_ptr(settings->AOColor));
        ImGui::InputFloat("AO intensity", &settings->AOIntensity, 0.001f, 0.001f, "%.3f");

        ImGui::InputFloat("Fog curve", &settings->fogCurve, 0.1f, 0.1f, "%.1f");
        ImGui::InputFloat("Fog start", &settings->fogStartDist, 0.1f, 0.1f, "%.1f");
    }

    if (ImGui::CollapsingHeader("Fractals")) {
        ImGui::InputFloat("Min dist", &settings->minDist, 0.0001f, 0.0001f, "%.4f");
        ImGui::InputInt("Iterations", &settings->iterations);
        ImGui::InputFloat("Power(mandel bulb)", &settings->fractalPower, 0.01f, 0.01f, "%.3f");
        ImGui::Combo("Fractal type", &settings->fractalType, "Meneger box\0Mandel box\0Mandel bulb\0Meneger sponge\0Serpinski triangle\0\0");

        if (ImGui::Button("Reset to default") || (settings->fractalType != previousFractalType)) {
            settings->zoom = 0.0f;
            previousFractalType = settings->fractalType;

            switch(settings->fractalType) {
                case 0:
                    settings->iterations = 8;
                    settings->cameraOrigin.z = 10.0f;
                    break;
                case 1:
                    settings->iterations = 8;
                    settings->cameraOrigin.z = 30.0f;
                    break;
                case 2:
                    settings->iterations = 12;
                    settings->fractalPower = 8.0;
                    settings->cameraOrigin.z = 4.0f;
                    break;
                case 3:
                    settings->iterations = 4;
                    settings->cameraOrigin.z = 10.0f;
                    break;
                case 4:
                    settings->iterations = 14;
                    settings->cameraOrigin.z = 10.0f;
                    break;
            }
        }
    }


    ImGui::End();


    return flags;
}