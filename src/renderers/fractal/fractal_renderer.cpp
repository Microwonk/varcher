#include <renderers/common/performance_gui.h>

#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include "fractal_renderer.h"

#include "engine/engine.h"
#include "fractal_settings_gui.h"

FractalRenderer::FractalRenderer(const std::shared_ptr<Engine>& engine) : ARenderer(engine)
{
    _pipeline = std::make_unique<FractalPipeline>(FractalPipeline::build(engine, _windowRenderPass->renderPass));
    _settings = std::make_shared<ShaderSettings>();
    _p_settings = std::make_shared<PerformanceSettings>();
    _imguiRenderer = std::make_unique<ImguiRenderer>(engine, _windowRenderPass->renderPass);

    _settingsBuffer = std::make_unique<Buffer>(engine, sizeof(ShaderSettings), vk::BufferUsageFlagBits::eUniformBuffer,
                                              VMA_MEMORY_USAGE_CPU_TO_GPU, "Shader Settings Buffer");
    _settingsBuffer->copyData(&_settings, sizeof(ShaderSettings));

    _vertexBuffer = std::make_unique<Buffer>(engine, vertices.size() * sizeof(FractalVertex), vk::BufferUsageFlagBits::eVertexBuffer,
                                             VMA_MEMORY_USAGE_CPU_TO_GPU, "Fractal Quad Buffer");
    _vertexBuffer->copyData(&vertices, vertices.size() * sizeof(FractalVertex));

    engine->recreationQueue->push(RecreationEventFlags::TARGET_RESIZE, [&]() {
        _pipeline->descriptorSet->initBuffer(0, _settingsBuffer->buffer, _settingsBuffer->size,
                                             vk::DescriptorType::eUniformBuffer);
        // _pipeline->descriptorSet->initBuffer();

        return [=](const std::shared_ptr<Engine> &) {};
    });

    engine->recreationQueue->push(RecreationEventFlags::SWAPCHAIN_RECREATE, [&]() {
       _settings->viewportWidth = (float)engine->windowSize.x;
       _settings->viewportHeight = (float)engine->windowSize.y;
       _settings->aspectRatio = (float)engine->windowSize.x / (float)engine->windowSize.y;

       return [=](const std::shared_ptr<Engine> &) {};
    });
}

void FractalRenderer::update(float delta)
{
    _settings->view = glm::rotate(glm::mat4(1.0), glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    _settings->view = glm::rotate(_settings->view, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));

    _settings->zoom = zoom;

    _time += delta;
    _imguiRenderer->beginFrame();
    RecreationEventFlags flags = PerformanceGui::draw(delta, _p_settings) | FractalSettingsGui::draw(_settings);

    if (flags & RecreationEventFlags::SWAPCHAIN_RECREATE) {
        engine->updatePresentMode(_p_settings->presentMode);
    }
}

void FractalRenderer::recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame)
{

    // Start main renderpass
    vk::RenderPassBeginInfo renderpassInfo;
    renderpassInfo.renderPass = _windowRenderPass->renderPass;
    renderpassInfo.renderArea.offset = vk::Offset2D(0, 0);
    renderpassInfo.renderArea.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    renderpassInfo.framebuffer = _windowFramebuffers[swapchainImage].framebuffer;
    renderpassInfo.clearValueCount = 1;
    vk::ClearValue clearValue;
    clearValue.color.setFloat32({0.0, 0.0, 0.0, 1.0});
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

    _settingsBuffer->copyData(&_settings, sizeof(ShaderSettings));
    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _settingsBuffer->buffer, sizeof(ShaderSettings), vk::DescriptorType::eUniformBuffer);
    commandBuffer.bindVertexBuffers(0, static_cast<vk::Buffer>(_vertexBuffer->buffer), static_cast<VkDeviceSize>(0ul));
    commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline->layout,
                                     0, 1,
                                     _pipeline->descriptorSet->getSet(flightFrame),
                                     0, nullptr);

    commandBuffer.draw((uint32_t)vertices.size(), 1, 0, 0);
    _imguiRenderer->draw(commandBuffer);

    // End main renderpass
    commandBuffer.endRenderPass();
}
