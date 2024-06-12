#pragma once

#include <renderers/common/performance_settings.h>
#include <engine/renderer.h>
#include <engine/gui/imgui_renderer.h>
#include <renderers/galaxy/stages/grid_stage.h>

class GalaxyRenderer : public ARenderer {
private:
    std::shared_ptr<PerformanceSettings> _p_settings;
    std::unique_ptr<ImguiRenderer> _imguiRenderer;

    float _time = 0;
    GameCamera _gameCamera = {};

    std::unique_ptr<GridStage> _gridStage;

public:
    explicit GalaxyRenderer(const std::shared_ptr<Engine>& engine);
    void update(float delta) override;
    void recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) override;

};
