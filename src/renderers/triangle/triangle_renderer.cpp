#include <renderers/common/performance_gui.h>
#include "triangle_renderer.h"

#include "engine/engine.h"

TriangleRenderer::TriangleRenderer(const std::shared_ptr<Engine>& engine) : ARenderer(engine)
{
    _pipeline = std::make_unique<TrianglePipeline>(engine, _windowRenderPass->renderPass);
    _p_settings = std::make_shared<PerformanceSettings>();
    _imguiRenderer = std::make_unique<ImguiRenderer>(engine, _windowRenderPass->renderPass);
    _pipeline->buildAll();
}

void TriangleRenderer::update(float delta)
{
    _time += delta;
    _imguiRenderer->beginFrame();
    RecreationEventFlags flags = PerformanceGui::draw(delta, _p_settings);

    if (flags & RecreationEventFlags::SWAPCHAIN_RECREATE) {
        engine->updatePresentMode(_p_settings->presentMode);
    }
}

void TriangleRenderer::recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t)
{
    // Create clear color for this frame
    vk::ClearValue clearValue;
    float flash = abs(sin(_time));
    clearValue.color = vk::ClearColorValue(std::array<float, 4> {0.0f, 0.0f, flash, 1.0f});

    // Start main renderpass
    vk::RenderPassBeginInfo renderpassInfo;
    renderpassInfo.renderPass = _windowRenderPass->renderPass;
    renderpassInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderpassInfo.renderArea.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    renderpassInfo.framebuffer = _windowFramebuffers[swapchainImage].framebuffer;
    renderpassInfo.clearValueCount = 1;
    renderpassInfo.pClearValues = &clearValue;
    commandBuffer.beginRenderPass(renderpassInfo, vk::SubpassContents::eInline);

    // Bind pipeline
    commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline->pipeline);

    // Set viewport
    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engine->windowSize.x);
    viewport.height = static_cast<float>(engine->windowSize.y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer.setViewport(0, 1, &viewport);

    // Set scissor
    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D(0, 0);
    scissor.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    commandBuffer.setScissor(0, 1, &scissor);

    commandBuffer.draw(3, 1, 0, 0);
    _imguiRenderer->draw(commandBuffer);

    // End main renderpass
    commandBuffer.endRenderPass();
}
