#include <renderers/common/performance_gui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "galaxy_renderer.h"
#include "engine/commands/command_util.h"

GalaxyRenderer::GalaxyRenderer(const std::shared_ptr<Engine> &engine) : ARenderer(engine) {

    _gridStage = std::make_unique<GridStage>(engine, *_windowRenderPass);
    _p_settings = std::make_shared<PerformanceSettings>();

    _imguiRenderer = std::make_unique<ImguiRenderer>(engine, _windowRenderPass->renderPass);
}

void GalaxyRenderer::update(float delta) {
    _time += delta;
    _imguiRenderer->beginFrame();
    RecreationEventFlags flags = PerformanceGui::draw(delta, _p_settings);

    _gameCamera.update(delta, engine);

    if (flags & RecreationEventFlags::SWAPCHAIN_RECREATE) {
        engine->updatePresentMode(_p_settings->presentMode);
    }
}

void GalaxyRenderer::recordCommands(const vk::CommandBuffer &commandBuffer, uint32_t swapchainImage, uint32_t flightFrame) {
    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engine->windowSize.x);
    viewport.height = static_cast<float>(engine->windowSize.y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D(0, 0);
    scissor.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    commandBuffer.setScissor(0, 1, &scissor);

    _gridStage->record(commandBuffer,
                       flightFrame,
                       _windowFramebuffers[swapchainImage],
                       *_windowRenderPass,
                       [=](const vk::CommandBuffer &cmd) {_imguiRenderer->draw(cmd);},
                       GridParams{ _gameCamera, CAMERA_FOV });
}
