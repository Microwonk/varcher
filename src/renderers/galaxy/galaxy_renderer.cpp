#include <renderers/common/performance_gui.h>
#include <glm/gtc/matrix_transform.hpp>
#include "galaxy_renderer.h"
#include "engine/commands/command_util.h"

GalaxyRenderer::GalaxyRenderer(const std::shared_ptr<Engine> &engine) : ARenderer(engine) {

    uint32_t recreatorId = engine->recreationQueue->push(RecreationEventFlags::RENDER_RESIZE, [&]() {
        glm::uvec2 renderRes = {1920., 1080.};

        _colorTarget = std::make_unique<RenderImage>(engine, renderRes.x, renderRes.y, vk::Format::eR8G8B8A8Unorm,
                                                     vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, vk::ImageAspectFlagBits::eColor, "Color Target");
        _depthTarget = std::make_unique<RenderImage>(engine, renderRes.x, renderRes.y, vk::Format::eR32Sfloat,
                                                     vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eSampled, vk::ImageAspectFlagBits::eColor, "Depth Target");

        return [=](const std::shared_ptr<Engine>&) {
            _colorTarget->destroy();
            _depthTarget->destroy();
        };
    });
    pushDeletor([=](const std::shared_ptr<Engine>& delEngine) {
        delEngine->recreationQueue->remove(recreatorId);
    });

    recreatorId = engine->recreationQueue->push(RecreationEventFlags::RENDER_RESIZE, [&]() {
        _renderPass = RenderPassBuilder(engine)
                .color(0, _colorTarget->format, glm::vec4(0.0))
                .color(1, _depthTarget->format, glm::vec4(0.0))
                .buildUnique("Geometry Render Pass");

        return [=](const std::shared_ptr<Engine>&) {
            _renderPass->destroy();
        };
    });
    pushDeletor([=](const std::shared_ptr<Engine>& delEngine) {
        delEngine->recreationQueue->remove(recreatorId);
    });

    recreatorId = engine->recreationQueue->push(RecreationEventFlags::RENDER_RESIZE, [&]() {
        _framebuffers = ResourceRing<Framebuffer>::fromFunc(2, [&](uint32_t n) {
            return FramebufferBuilder(engine, _renderPass->renderPass, {1920., 1080.})
                    .color(_colorTarget->imageView)
                    .color(_depthTarget->imageView)
                    .build("Galaxy Framebuffer");
        });

        return [=](const std::shared_ptr<Engine>&) {
            _framebuffers.destroy([&](const Framebuffer& framebuffer) {
                framebuffer.destroy();
            });
        };
    });

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

    //gridStage
    int32_t numCells = 16;

    float aspect = (float)engine->windowSize.x / (float)engine->windowSize.y;
    if(aspect < 1.0f)
        aspect = 1.0f / aspect;

    float size = aspect * powf(2.0f, roundf(log2f(_gameCamera.dist) + 0.5f));

    glm::vec3 pos = _gameCamera.center;
    for(int32_t i = 0; i < 3; i++)
        pos[i] -= fmodf(pos[i], size / numCells);

    glm::mat4 model = glm::translate(glm::mat4(1.0), pos);
    model *= glm::scale(model, glm::vec3(size, size, size));

    GridPush constants{};
    constants.model = model;
    constants.thickness = 0.0125f;
    constants.scroll = (_gameCamera.dist - powf(2.0f, roundf(log2f(_gameCamera.dist) - 0.5f))) / (4.0f * powf(2.0f, roundf(log2f(_gameCamera.dist) - 1.5f))) + 0.5f;
    glm::vec3 offset3 = (_gameCamera.center - pos) / size;
    constants.offset = glm::vec2(offset3.x, offset3.z);
    constants.numCells = numCells;

    commandBuffer.pushConstants(_gridStage->getPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(GridPush), &constants);

    _gridStage->record(commandBuffer,
                       flightFrame,
                       _windowFramebuffers[swapchainImage],
                       *_windowRenderPass,
                       [=](const vk::CommandBuffer &cmd) {_imguiRenderer->draw(cmd);},
                       GridParams{ _gameCamera, CAMERA_FOV });
}
