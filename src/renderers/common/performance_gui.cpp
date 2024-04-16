#include "performance_gui.h"

#include <imgui.h>
#include <fmt/format.h>
#include <algorithm>
#include <iostream>

const std::vector<VkPresentModeKHR> presentmodeOptions = {
        VkPresentModeKHR::VK_PRESENT_MODE_IMMEDIATE_KHR,
        VkPresentModeKHR::VK_PRESENT_MODE_MAILBOX_KHR,
        VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR,
        VkPresentModeKHR::VK_PRESENT_MODE_FIFO_RELAXED_KHR
};

RecreationEventFlags PerformanceGui::draw(float delta, const std::shared_ptr<PerformanceSettings>& settings)
{
    static float delta_accum = 0;
    static float last_fps = 0;
    delta_accum += delta;

    RecreationEventFlags flags;
    static float ft_history[25];
    std::rotate(std::begin(ft_history), std::next(std::begin(ft_history)), std::end(ft_history));
    ft_history[0] = delta;

    ImGui::Begin("Performance");

    if (ImGui::BeginCombo("Present Mode", settings->presentModeName().c_str()))
    {
        for (const VkPresentModeKHR mode: presentmodeOptions)
        {
            if (ImGui::Selectable(settings->presentModeNameStr(mode).c_str(), settings->presentMode == mode))
            {
                settings->presentMode = mode;
                // std::cout << settings->presentModeNameStr(mode).c_str() << std::endl;
                flags |= RecreationEventFlags::SWAPCHAIN_RECREATE;
            }

            if (settings->presentMode == mode)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    // the fps gets updated
    if (delta_accum > 0.5) {
        last_fps = 1.0f / delta;
        delta_accum = 0;
    }
    ImGui::LabelText("FPS", "%s", fmt::format("{:.0f}", last_fps).c_str());
    ImGui::LabelText("Frame Time", "%s", fmt::format("{}", delta * 1000).c_str());
    ImGui::PlotHistogram("Frame Time History", ft_history, 25, 0, nullptr, 0, 1.0f / 30.0f, ImVec2(0, 80));
    ImGui::End();

    return flags;
}
