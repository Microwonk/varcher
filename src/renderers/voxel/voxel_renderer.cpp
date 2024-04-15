#include "voxel_renderer.h"

#include "engine/engine.h"
#include "voxel_settings_gui.h"
#include "engine/commands/command_util.h"
#include "renderers/voxel/resource/screen_quad_push.h"
#include "engine/resource/render_image.h"
#include "renderers/common/performance_gui.h"

VoxelRenderer::VoxelRenderer(const std::shared_ptr<Engine>& engine) : ARenderer(engine)
{
    _v_settings = std::make_shared<VoxelRenderSettings>();
    _p_settings = std::make_shared<PerformanceSettings>();

    _camera = std::make_unique<CameraController>(engine, glm::vec3(8, 8, -50), 90.0f, 0.0f, static_cast<float>(1 / glm::tan(glm::radians(55.0f / 2))));

    _noiseTexture = std::make_shared<Texture2D>(engine, "../resource/blue_noise_rgba.png", 4, vk::Format::eR8G8B8A8Unorm);
    _scene = std::make_shared<VoxelScene>(engine, _v_settings->voxPath, _v_settings->skyboxPath);

    _geometryStage = std::make_unique<GeometryStage>(engine, _v_settings, _scene, _noiseTexture);
#ifdef _WIN32
    _upscalerStage = std::make_unique<UpscalerStage>(engine, _v_settings);
#endif
    _blitStage = std::make_unique<BlitStage>(engine, _v_settings, *_windowRenderPass);

    _imguiRenderer = std::make_unique<ImguiRenderer>(engine, _windowRenderPass->renderPass);
}

void VoxelRenderer::update(float delta)
{
    _time += delta;

    _camera->update(delta);

#ifdef _WIN32
    _upscalerStage->update(delta);
#endif
    _imguiRenderer->beginFrame();
    RecreationEventFlags flags = VoxelSettingsGui::draw(_v_settings) | PerformanceGui::draw(delta, _p_settings);
    engine->recreationQueue->fire(flags);
    if (flags & RecreationEventFlags::SCENE_PATH) {
        _scene->destroy();
        _scene = std::make_shared<VoxelScene>(engine, _v_settings->voxPath, _v_settings->skyboxPath);

        _geometryStage->destroy();
        _geometryStage = std::make_unique<GeometryStage>(engine, _v_settings, _scene, _noiseTexture);
    }

    if (flags & RecreationEventFlags::SWAPCHAIN_RECREATE) {
        engine->updatePresentMode(_p_settings->presentMode);
    }
}

void VoxelRenderer::recordCommands(const vk::CommandBuffer& commandBuffer, uint32_t swapchainImage, uint32_t flightFrame)
{
    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(_v_settings->renderResolution().x);
    viewport.height = static_cast<float>(_v_settings->renderResolution().y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    commandBuffer.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D(0, 0);
    scissor.extent = vk::Extent2D(_v_settings->renderResolution().x, _v_settings->renderResolution().y);
    commandBuffer.setScissor(0, 1, &scissor);

    // Set push constants
    ScreenQuadPush constants{};
    constants.screenSize = glm::ivec2(_v_settings->renderResolution().x, _v_settings->renderResolution().y);
    constants.volumeBounds = glm::uvec3(_scene->width, _scene->height, _scene->depth);
    constants.camPos = glm::vec4(_camera->position, 1);
    constants.camDir = glm::vec4(_camera->direction, 0);
    constants.camUp = glm::vec4(_camera->up, 0);
    constants.camRight = glm::vec4(_camera->right, 0);
#ifdef _WIN32
    constants.frame = glm::uvec1(_upscalerStage->frameCount);
    constants.cameraJitter.x = _upscalerStage->jitterX;
    constants.cameraJitter.y = _upscalerStage->jitterY;
#endif
    commandBuffer.pushConstants(_geometryStage->getPipelineLayout(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment, 0, sizeof(ScreenQuadPush), &constants);

    const GeometryBuffer& gBuffer = _geometryStage->record(commandBuffer, flightFrame);

    const RenderImage& color = gBuffer.color.get();

#ifdef _WIN32
    const RenderImage& upscaled = _v_settings->fsrSettings.enable ? _upscalerStage->record(commandBuffer,
                                                                                           color, gBuffer.depth, gBuffer.motion, gBuffer.mask) : color;

    _blitStage->record(commandBuffer, flightFrame, upscaled, _windowFramebuffers[swapchainImage], *_windowRenderPass, [=](const vk::CommandBuffer& cmd) {_imguiRenderer->draw(cmd);});
#else
    _blitStage->record(commandBuffer, flightFrame, color, _windowFramebuffers[swapchainImage], *_windowRenderPass, [=](const vk::CommandBuffer &cmd) {_imguiRenderer->draw(cmd);});
#endif
    cmdutil::imageMemoryBarrier(
        commandBuffer,
        engine->swapchain.images[swapchainImage],
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eNone,
        vk::ImageLayout::eColorAttachmentOptimal,
        vk::ImageLayout::ePresentSrcKHR,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eBottomOfPipe,
        vk::ImageAspectFlagBits::eColor);
}
