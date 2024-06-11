#include "grid_stage.h"
#include "engine/commands/command_util.h"

const vk::PipelineLayout &GridStage::getPipelineLayout() const {
    return _pipeline->layout;
}

GridStage::GridStage(const std::shared_ptr<Engine> &engine, const RenderPass &renderPass) : AResource(engine) {

    _cameraBuffer = std::make_unique<Buffer>(engine, sizeof(Camera), vk::BufferUsageFlagBits::eUniformBuffer,
                                             VMA_MEMORY_USAGE_CPU_TO_GPU, "Grid Camera Buffer");

    _pipeline = std::make_unique<GridPipeline>(GridPipeline::build(engine, renderPass.renderPass));

    engine->recreationQueue->push(RecreationEventFlags::TARGET_RESIZE, [&]() {
        _pipeline->descriptorSet->initBuffer(0, _cameraBuffer->buffer, _cameraBuffer->size,
                                             vk::DescriptorType::eUniformBuffer);

        return [=](const std::shared_ptr<Engine> &) {};
    });
}

void GridStage::record(const vk::CommandBuffer &cmd, uint32_t flightFrame,
                       const Framebuffer &windowFramebuffer, const RenderPass &windowRenderPass,
                       std::function<void(const vk::CommandBuffer &)> uiStage) {

    vk::Viewport viewport;
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(engine->windowSize.x);
    viewport.height = static_cast<float>(engine->windowSize.y);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    cmd.setViewport(0, 1, &viewport);

    vk::Rect2D scissor;
    scissor.offset = vk::Offset2D(0, 0);
    scissor.extent = vk::Extent2D(engine->windowSize.x, engine->windowSize.y);
    cmd.setScissor(0, 1, &scissor);

    _camera.u_proj = glm::mat4();
    _camera.u_view = glm::mat4();
    _camera.u_viewProj = glm::mat4();

    _cameraBuffer->copyData(&_camera, sizeof(Camera));

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(Camera),
                                          vk::DescriptorType::eUniformBuffer);

    windowRenderPass.recordBegin(cmd, windowFramebuffer);
    cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline->pipeline);

    _pipeline->descriptorSet->writeBuffer(0, flightFrame, _cameraBuffer->buffer, sizeof(Camera), vk::DescriptorType::eUniformBuffer);

    cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipeline->layout,
                           0, 1,
                           _pipeline->descriptorSet->getSet(flightFrame),
                           0, nullptr);

    cmd.draw(3, 1, 0, 0);
    uiStage(cmd);
    cmd.endRenderPass();

}
