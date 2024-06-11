#include <renderers/common/performance_gui.h>
#include "galaxy_renderer.h"

GalaxyRenderer::GalaxyRenderer(const std::shared_ptr<Engine> &engine) : ARenderer(engine) {
    _gridStage = std::make_unique<GridStage>(engine, *_windowRenderPass);
    _p_settings = std::make_shared<PerformanceSettings>();

    _imguiRenderer = std::make_unique<ImguiRenderer>(engine, _windowRenderPass->renderPass);
}

void GalaxyRenderer::update(float delta) {
    _time += delta;
    _imguiRenderer->beginFrame();
    RecreationEventFlags flags = PerformanceGui::draw(delta, _p_settings);

    if (flags & RecreationEventFlags::SWAPCHAIN_RECREATE) {
        engine->updatePresentMode(_p_settings->presentMode);
    }
}

void GalaxyRenderer::recordCommands(const vk::CommandBuffer &commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) {
    _gridStage->record(commandBuffer, flightFrame, _windowFramebuffers[swapchainImage], *_windowRenderPass, [=](const vk::CommandBuffer &cmd) {_imguiRenderer->draw(cmd);});
}
